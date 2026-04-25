# Steganography with OpenCV

[cite_start]This educational project demonstrates data hiding in digital images using the OpenCV library[cite: 1, 5, 25]. [cite_start]The project is written in C++17 and explores LSB (Least Significant Bit) and MSB (Most Significant Bit) encoding techniques[cite: 14, 16, 25].

[cite_start]The main goal is to hide a payload (a Bash script) within the bits of a PNG image (strictly lossless), and then automatically extract and execute it[cite: 17, 18, 86].

---

## Technologies Used
* [cite_start]**Language:** C++17 [cite: 14]
* [cite_start]**Library:** OpenCV 4.x [cite: 15]
* [cite_start]**Payload:** Bash Script [cite: 17]

---

## Project Architecture

[cite_start]The project is structured around a single source file that exposes five essential functionalities[cite: 37]:

* [cite_start]`encodeLSB()`: Encodes the bash script into the LSB bits of the blue (B) channel[cite: 38]. [cite_start]A modification of $\pm1$ out of 255 is completely invisible to the human eye[cite: 41]. [cite_start]The generated image is visually identical to the original[cite: 34].
* [cite_start]`encodeMSB()`: Encodes the script into the MSB bits (weight 128)[cite: 66]. [cite_start]This function produces major visual distortions and is included exclusively to demonstrate the impact of modifying significant bits[cite: 38, 66].
* [cite_start]`showRichHistogram()`: Calculates and displays RGB histograms to compare the original image with the stego image[cite: 38].
* [cite_start]`showLSBPlane()`: Extracts and visualizes the LSB bit plane[cite: 38]. [cite_start]It allows observing the hidden data patterns compared to the natural noise of the original image[cite: 81].
* [cite_start]`extractAndRun()`: Extracts the bash script from the stego image, saves it to disk (`extracted.sh`), and executes it using the `system()` call[cite: 38, 86, 88].

---

## Visual Comparison

* [cite_start]**LSB (Least Significant Bit):** The modifications are visually imperceptible, making it ideal for real steganography[cite: 69].
* [cite_start]**MSB (Most Significant Bit):** Produces major chromatic artifacts due to the modification of the $2^{7}=128$ weight bit, making it highly visible and immediately detectable[cite: 69, 116].

*(I recommend adding your demonstration images here. For example:)*
---

## Security Notes and Limitations

* [cite_start]**Execution Risk:** Using `system()` to directly execute a script extracted from an image represents a real attack vector[cite: 89]. [cite_start]This project is purely demonstrative[cite: 92]. [cite_start]In production systems, no image should be executed without rigorous validation[cite: 90].
* [cite_start]**MSB Technique:** It is for EXCLUSIVELY EDUCATIONAL purposes[cite: 67]. [cite_start]Any observer would immediately detect the altered image[cite: 67].
* [cite_start]**JPEG Compression:** The PNG format is mandatory[cite: 18]. [cite_start]Saving in lossy formats (such as JPEG) destroys the LSB data through recompression, eliminating any hidden payload[cite: 120, 138].

---

## How to Run the Project
*(Fill in your specific compilation steps here, e.g.: `g++ main.cpp -o stego $(pkg-config --cflags --libs opencv4)`)*
