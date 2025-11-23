#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link against Windows Socket Library
#pragma comment(lib, "ws2_32.lib")

#define HOST "127.0.0.1"
#define PORT 5000

// --- SYSTEM CONTEXT ---
std::string SYSTEM_CONTEXT = R"(
### ROLE
You are an expert Lua programmer scripting for a specific custom engine called "LuaEmbed". You must write code that generates pixel art animations using *only* the provided API functions.

### COORDINATE SYSTEM & COLORS
- **Origin:** (0, 0) is the top-left pixel.
- **Indexing:** 0-based indexing (x ranges from 0 to width-1).
- **Colors:** Defined as tables `{r=0..255, g=0..255, b=0..255}`.
- **Constraints:** The engine throws errors for invalid inputs. All coordinates (`x`, `y`) must be **non-negative integers** (>= 0). All dimensions (`w`, `h`) must be **positive integers** (> 0). You must manually clamp or check coordinates before passing them to API functions (especially `pasteRegion` and `moveRegion`) to prevent negative values.

### API REFERENCE
You have access to the standard Lua `math` library and the following custom functions:

1. `getWidth() -> integer`
   Returns canvas width.
2. `getHeight() -> integer`
   Returns canvas height.
3. `setPixel(x, y, color_table)`
   Draws a single pixel.
4. `getPixel(x, y) -> table`
   Returns `{r, g, b, a}` of a pixel.
5. `defineFrames(total_frames)`
   Initializes the animation buffer size. Call this second.
6. `setCurrentFrame(frame_index)`
   Target a specific frame to draw on (1-based index for frames).
7. `copyRegion(x, y, w, h)`
   Copies pixels from the current frame into an internal clipboard. `x, y` must be >= 0. `w, h` must be > 0.
8. `pasteRegion(x, y)`
   Pastes the clipboard contents onto the current frame at (x, y). `x, y` must be >= 0.
9. `moveRegion(x, y, w, h, target_x, target_y)`
   Moves a block of pixels from one location to another within the current frame. All coordinates must be >= 0.
10. `setFrameDelay(delay_in_millisecond: integer)`
   Sets a delay on current frame.
11. `defineCanvas(width, height)`
   Initializes the canvas width and height size non negative but both must be less than 50. Call this first.

### ANIMATION STRATEGY
To create complex animations (moving object + internal movement like walking/bouncing), follow this pattern:
1. Call `defineCanvas`.
2. Call `defineFrames`
3. **Asset Strategy:**
   - **Option A (Composite):** Draw a static "body" and `copyRegion` it. In the loop, `pasteRegion` the body, then use `setPixel` to draw dynamic parts (legs, eyes) relative to the body based on the frame index.
   - **Option B (Procedural):** Create a local Lua function to draw the sprite. Pass `x, y` and a `state` variable (e.g., leg position) to this function inside the loop.
4. **The Loop:** Iterate through all frames.
5. **Calculations:**
   - Calculate global position (x, y) using math (sin/cos/linear).
   - Calculate internal state (e.g., `leg_offset = math.sin(frame * 0.5)`).
6. **Safety:** Apply `math.floor` and `math.max(0, ...)` to all coordinates to ensure they are integers and strictly non-negative.
7. **Render:** Draw the sprite at the safe coordinates using the chosen Asset Strategy.

### TASK
Create a seamless loop animation of a colorful object moving across the screen.
- The object must exhibit **internal animation** (e.g., moving limbs, changing shape, blinking, or spinning elements) in addition to its path of movement.
- Output **ONLY** valid Lua code.
- Do not include explanations or comments.
- Frame count cannot be greater than 100.
- Make visually appealing animations for humans.
- Make sure no function gets a nil value
)";

// --- HELPER 1: Escape string for JSON ---
std::string json_escape(const std::string& s) {
    std::string output;
    for (char c : s) {
        switch (c) {
            case '\"': output += "\\\""; break;
            case '\\': output += "\\\\"; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;
            default: output += c;
        }
    }
    return output;
}

// --- HELPER 2: Strip Markdown (```lua ... ```) ---
std::string strip_markdown(std::string text) {
    // 1. Remove starting ```lua or ```
    size_t start_marker = text.find("```");
    if (start_marker == 0) { // Only if it starts exactly at 0
        size_t line_end = text.find('\n', start_marker);
        if (line_end != std::string::npos) {
            text = text.substr(line_end + 1); // Skip the first line
        }
    }

    // 2. Remove ending ```
    size_t end_marker = text.rfind("```");
    if (end_marker != std::string::npos) {
        text = text.substr(0, end_marker);
    }

    return text;
}

// --- HELPER 3: Extract "solution" from JSON ---
std::string extract_solution(const std::string& json_response) {
    std::string key = "\"solution\"";
    size_t start_pos = json_response.find(key);

    if (start_pos == std::string::npos) return "Error: No 'solution' found.";

    // Find start of value
    start_pos = json_response.find("\"", json_response.find(":", start_pos));
    if (start_pos == std::string::npos) return "Error: Malformed JSON";
    start_pos++;

    std::string result = "";
    bool escape = false;

    // Iterate until closing quote
    for (size_t i = start_pos; i < json_response.length(); ++i) {
        char c = json_response[i];
        if (escape) {
            switch (c) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case '\"': result += '\"'; break;
                case '\\': result += '\\'; break;
                default: result += c; break;
            }
            escape = false;
        } else {
            if (c == '\\') escape = true;
            else if (c == '\"') break; // End of string
            else result += c;
        }
    }

    // Clean the result before returning
    return strip_markdown(result);
}

// --- MAIN FUNCTION: PROMPT ---
std::string prompt(std::string user_request) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return "Error: WSAStartup";

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) { WSACleanup(); return "Error: Socket creation"; }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &server.sin_addr);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        closesocket(sock); WSACleanup(); return "Error: Connection failed";
    }

    // Build Prompt
    std::string full_prompt = SYSTEM_CONTEXT + "\nUSER REQUEST: " + user_request + "\nOutput raw Lua code only. No markdown formatting.";
    std::string jsonBody = "{\"prompt\": \"" + json_escape(full_prompt) + "\"}";

    // Build HTTP Request
    std::ostringstream request;
    request << "POST /api/solve HTTP/1.1\r\n";
    request << "Host: " << HOST << "\r\n";
    request << "Content-Type: application/json\r\n";
    request << "Content-Length: " << jsonBody.length() << "\r\n";
    request << "Connection: close\r\n\r\n";
    request << jsonBody;

    send(sock, request.str().c_str(), request.str().length(), 0);

    // Receive
    std::string response;
    char buffer[4096];
    int bytesReceived;
    while ((bytesReceived = recv(sock, buffer, 4096, 0)) > 0) {
        response.append(buffer, bytesReceived);
    }

    closesocket(sock);
    WSACleanup();

    // Extract
    size_t headerEnd = response.find("\r\n\r\n");
    if (headerEnd != std::string::npos) {
        return extract_solution(response.substr(headerEnd + 4));
    }
    return "Error: Invalid response";
}

// --- USAGE ---
int e() {
    // This returns PURE Lua code now (no ```)
    std::string code = prompt("make a ghost animation");
    std::cout << code << std::endl;
    return 0;
}