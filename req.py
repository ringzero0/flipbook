import requests
import json


URL = "http://localhost:5000/api/solve"


SYSTEM_CONTEXT = """
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
- Frame Count cannot be greater than 100.
- Make visually appealing animation for humans.
"""


payload = {
    "prompt": SYSTEM_CONTEXT + "make diwali animation"
}

print("Sending request ")

try:
    response = requests.post(URL, json=payload)
    

    if response.status_code == 200:
        data = response.json()
        if data.get("success"):
            print("\n --- Generated Lua Code --- \n")
            print(data['solution'])
            print("\n-----------------------------")
            
            # Optional: Save to file
            with open("animation.lua", "w") as f:
                f.write(data['solution'])
            print(" Saved to animation.lua")
        else:
            print(" Server Error:", data.get("error"))
            if "details" in data:
                print("Details:", data['details'])
    else:
        print(f" HTTP Error {response.status_code}: {response.text}")

except Exception as e:
    print(f" Connection Failed: {e}")
    print("Make sure app.py is running on localhost:5000")