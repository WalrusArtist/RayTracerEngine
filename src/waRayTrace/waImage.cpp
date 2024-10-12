
/*
    The `waImage` class is responsible for managing and manipulating image data in the context of a ray tracing application, which renders images pixel-by-pixel. This class handles setting individual pixel colors, initializing textures using SDL (Simple DirectMedia Layer), and rendering the image to the screen.

    1. **Constructor and Destructor**:
       - **Constructor (`waImage::waImage`)**:
         - Initializes the image dimensions (`m_xSize` and `m_ySize`) to zero.
         - Sets the texture pointer (`m_pTexture`) to `NULL`, indicating that no texture has been created yet.
       
       - **Destructor (`waImage::~waImage`)**:
         - If a texture exists (`m_pTexture != NULL`), it is destroyed using `SDL_DestroyTexture()` to free up memory.

    2. **Image Initialization (`waImage::Initialize`)**:
       - This method initializes the image with the specified dimensions (`xSize` and `ySize`) and binds it to the provided SDL renderer (`pRenderer`).
       - **Color Channels**:
         - Three 2D vectors (`m_rChannel`, `m_gChannel`, `m_bChannel`) are created to store the red, green, and blue color values for each pixel. These vectors are resized to the given image dimensions and initialized to `0.0` (black).
       - **Renderer and Texture**:
         - The SDL renderer is stored in `m_pRenderer`, and the texture is initialized using `InitTexture()`.

    3. **Setting Pixel Color (`waImage::SetPixel`)**:
       - This function sets the color of an individual pixel at coordinates `(x, y)` using the specified red, green, and blue values.
       - The color values are stored in the respective channels (`m_rChannel`, `m_gChannel`, `m_bChannel`) using the `at()` function for bounds checking.
       
    4. **Image Size Retrieval (`waImage::GetXSize`, `waImage::GetYSize`)**:
       - These getter methods return the image width (`m_xSize`) and height (`m_ySize`), respectively.

    5. **Displaying the Image (`waImage::Display`)**:
       - **Pixel Buffer (`tempPixel`)**:
         - A temporary array (`tempPixel`) is created to store the image data as a 1D array of `Uint32` values, where each `Uint32` represents the color of a pixel.
         - The array is initialized to zero using `memset()`, ensuring the image starts off as black.

       - **Color Conversion**:
         - For each pixel `(x, y)`, the method converts the red, green, and blue values from the 2D color channels into a single `Uint32` value using the `ConvertColor()` function. This converted color is stored in the `tempPixel` array at the corresponding position.

       - **Texture Update**:
         - After processing all pixels, the `SDL_UpdateTexture()` function is called to update the SDL texture (`m_pTexture`) with the newly computed pixel colors from `tempPixel`.

       - **Rendering the Texture**:
         - The texture is rendered onto the screen using `SDL_RenderCopy()`. The `srcRect` and `bounds` are set to cover the entire image size.

    6. **Texture Initialization (`waImage::InitTexture`)**:
       - This function creates an SDL texture that will be used to display the image. It handles both little-endian and big-endian systems by setting appropriate masks for red, green, blue, and alpha channels.
       
       - **Endianness Handling**:
         - Depending on the system's byte order (`SDL_BYTEORDER`), the red, green, blue, and alpha masks (`rmask`, `gmask`, `bmask`, `amask`) are set accordingly.
       
       - **Texture Creation**:
         - First, any previously created texture is destroyed using `SDL_DestroyTexture()` to avoid memory leaks.
         - A temporary surface (`SDL_Surface`) is created using `SDL_CreateRGBSurface()` with the image dimensions (`m_xSize`, `m_ySize`) and the appropriate color masks.
         - The surface is then converted into a texture using `SDL_CreateTextureFromSurface()`, and the surface is freed using `SDL_FreeSurface()`.

    7. **Color Conversion (`waImage::ConvertColor`)**:
       - This method converts red, green, and blue values (stored as `double`s) into a single `Uint32` value for use with SDL.
       
       - **Color Range**:
         - The method clamps the red, green, and blue values to the range `[0, 255]` by casting them to `unsigned char` (8-bit per channel).
       
       - **Endianness Handling**:
         - On big-endian systems, the red, green, and blue channels are shifted into the most significant bytes of the `Uint32` value, while alpha (transparency) is set to `255` (opaque).
         - On little-endian systems, the alpha channel is placed in the most significant byte, followed by blue, green, and red.

    8. **Summary**:
       - The `waImage` class encapsulates all the functionality required to manage an image in memory and render it using SDL. It handles pixel-level manipulation, texture creation, and efficient rendering.
       - The class also accounts for differences in system architecture (big-endian vs. little-endian) to ensure correct color representation on different platforms.
       - **Key Features**:
         - **Dynamic Image Resizing**: The image can be dynamically resized during initialization.
         - **Pixel Color Manipulation**: Pixels are set individually, making the class suitable for ray tracing, where each pixel is computed one by one.
         - **SDL Integration**: The class is tightly integrated with SDL, leveraging its texture and rendering functionalities for displaying images.
*/

#include "waImage.hpp"

waImage::waImage() {
    m_xSize = 0;
    m_ySize = 0;
    m_pTexture = NULL;
}

waImage::~waImage() {
    if (m_pTexture != NULL) {
        SDL_DestroyTexture(m_pTexture);
    }
}

void waImage::Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer) {
    m_rChannel.resize(xSize, std::vector<double>(ySize, 0.0));
    m_gChannel.resize(xSize, std::vector<double>(ySize, 0.0));
    m_bChannel.resize(xSize, std::vector<double>(ySize, 0.0));
    m_xSize = xSize;
    m_ySize = ySize;
    m_pRenderer = pRenderer;
    InitTexture();
}

void waImage::SetPixel(const int x, const int y, const double red, const double green, const double blue) {
    m_rChannel.at(x).at(y) = red;
    m_gChannel.at(x).at(y) = green;
    m_bChannel.at(x).at(y) = blue;
}

int waImage::GetXSize() { return m_xSize;}
int waImage::GetYSize() { return m_ySize;}

void waImage::Display() {
  ComputeMaxValues();
  Uint32 *tempPixel = new Uint32[m_xSize * m_ySize];
  memset(tempPixel, 0, m_xSize * m_ySize * sizeof(Uint32));

  for (int x = 0; x < m_xSize; ++x){
      for (int y = 0; y < m_ySize; ++y) {
          // convert the 2 dimentional x and y coords into a linear index into tempPixel.
          tempPixel[(y * m_xSize) + x] = ConvertColor(m_rChannel.at(x).at(y), m_gChannel.at(x).at(y), m_bChannel.at(x).at(y));
      }
  }

  SDL_UpdateTexture(m_pTexture, NULL, tempPixel, m_xSize * sizeof(Uint32));
  delete[] tempPixel;
  SDL_Rect srcRect, bounds;
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = m_xSize;
  srcRect.h = m_ySize;
  bounds = srcRect;
  SDL_RenderCopy(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

// func to initialize the texture
void waImage::InitTexture() {
    Uint32 rmask, gmask, bmask, amask;

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

    if (m_pTexture != NULL) {
        SDL_DestroyTexture(m_pTexture);
    }
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_xSize, m_ySize, 32, rmask, gmask, bmask, amask);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

Uint32 waImage::ConvertColor(const double red, const double green, const double blue) {
    unsigned char r = static_cast<unsigned char>((red / m_overallMax) * 255.0);
    unsigned char g = static_cast<unsigned char>((green / m_overallMax) * 255.0);
    unsigned char b = static_cast<unsigned char>((blue / m_overallMax) * 255.0);

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32 pixelColor = (r << 24) + (g << 16) + (b << 8) + 255;
    #else
        Uint32 pixelColor = (255 << 24) + (b << 16) + (g << 8) + r;
    #endif

    return pixelColor;
}

void waImage::ComputeMaxValues() {
	m_maxRed     = 0.0;
	m_maxGreen   = 0.0;
	m_maxBlue    = 0.0;
	m_overallMax = 0.0;
	for (int x=0; x<m_xSize; ++x) {
		for (int y=0; y<m_ySize; ++y) {
			double redValue		= m_rChannel.at(x).at(y);
			double greenValue	= m_gChannel.at(x).at(y);
			double blueValue	= m_bChannel.at(x).at(y);

			if (redValue > m_maxRed)
			  m_maxRed = redValue;
			
      if (greenValue > m_maxGreen)
				m_maxGreen = greenValue;
			
      if (blueValue > m_maxBlue)
				m_maxBlue = blueValue;
			
      if (m_maxRed > m_overallMax)
				m_overallMax = m_maxRed;
			
      if (m_maxGreen > m_overallMax)
				m_overallMax = m_maxGreen;
			
      if (m_maxBlue > m_overallMax)
				m_overallMax = m_maxBlue;
		}
	}
}
