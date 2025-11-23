import os
from flask import Flask, request, jsonify
import google.generativeai as genai
from dotenv import load_dotenv


load_dotenv()

app = Flask(__name__)


API_KEY = "YOUR API KEY"

if not API_KEY:
    raise ValueError("No GOOGLE_API_KEY found in environment variables")

genai.configure(api_key=API_KEY)

print("Fetching available models...")
try:

    for m in genai.list_models():
        if 'generateContent' in m.supported_generation_methods:
            print(f"Available: {m.name}")
            
except Exception as e:
    print(f"Error fetching models: {e}")


MODEL_NAME = 'gemini-pro-latest' 

try:
    model = genai.GenerativeModel(MODEL_NAME)
except Exception as e:
    print(f"Error initializing model: {e}")

@app.route('/status', methods=['GET'])
def status():

    return jsonify({"status": "running", "model": MODEL_NAME}), 200

@app.route('/api/solve', methods=['POST'])
def solve_prompt():

    data = request.get_json()


    if not data or 'prompt' not in data:
        return jsonify({"error": "Missing 'prompt' in JSON body"}), 400

    user_prompt = data['prompt']

    try:

        response = model.generate_content(user_prompt)
        

        solution_text = response.text

        return jsonify({
            "success": True,
            "solution": solution_text
        }), 200

    except Exception as e:
        return jsonify({
            "success": False,
            "error": str(e)
        }), 500

if __name__ == '__main__':

    app.run(host='0.0.0.0', port=5000, debug=True)