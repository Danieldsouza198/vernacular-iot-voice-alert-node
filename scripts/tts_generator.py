import argparse
import os
from gtts import gTTS

def generate_tts(text, lang, output_file):
    """Generates an MP3 file from text using Google TTS."""
    print(f"Generating {lang} audio for: '{text}'")
    try:
        tts = gTTS(text=text, lang=lang, slow=False)
        tts.save(output_file)
        print(f"✅ Successfully saved: {output_file}")
    except Exception as e:
        print(f"❌ Error generating TTS: {e}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate vernacular TTS audio for IoT alerts")
    parser.add_argument("--text", required=True, help="Text to convert to speech")
    parser.add_argument("--lang", default="hi", help="Language code (e.g., hi, ta, es, bn)")
    parser.add_argument("--output", default="alert.mp3", help="Output file path (e.g., data/flood_hi.mp3)")
    
    args = parser.parse_args()
    
    # Ensure output directory exists
    os.makedirs(os.path.dirname(args.output) if os.path.dirname(args.output) else '.', exist_ok=True)
    generate_tts(args.text, args.lang, args.output)