#define WIDTH 512
#define HEIGHT 512
#define MAX_HEIGHT 256
#define PERMUTATION_SIZE 256

#include <terrain.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

float lerp(float t, float a, float b) { return a + t * (b - a); }

float grad(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x
                                             : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

int permutation[PERMUTATION_SIZE * 2];

// Function to generate a permutation array based on a seed
void generatePermutation(int seed)
{
    int perm[PERMUTATION_SIZE];

    // Initialize permutation array with values from 0 to PERMUTATION_SIZE - 1
    for (int i = 0; i < PERMUTATION_SIZE; i++)
        perm[i] = i;

    // Shuffle the permutation array using the seed
    srand(seed);
    for (int i = PERMUTATION_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        // Swap perm[i] with perm[j]
        int temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }

    // Duplicate the permutation array to handle wrap-around
    for (int i = 0; i < PERMUTATION_SIZE; i++)
    {
        permutation[PERMUTATION_SIZE + i] = permutation[i] = perm[i];
    }
}

void initNoise(int seed)
{
    generatePermutation(seed);
}

// Perlin noise function for 3D space
float PerlinNoise3D(float x, float y, float z)
{
    int X = (int)floor(x) & 255,
        Y = (int)floor(y) & 255,
        Z = (int)floor(z) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    float u = fade(x),
          v = fade(y),
          w = fade(z);
    int A = permutation[X] + Y, AA = permutation[A] + Z, AB = permutation[A + 1] + Z,
        B = permutation[X + 1] + Y, BA = permutation[B] + Z, BB = permutation[B + 1] + Z;
    return lerp(w, lerp(v, lerp(u, grad(permutation[AA], x, y, z), grad(permutation[BA], x - 1, y, z)), lerp(u, grad(permutation[AB], x, y - 1, z), grad(permutation[BB], x - 1, y - 1, z))),
                lerp(v, lerp(u, grad(permutation[AA + 1], x, y, z - 1), grad(permutation[BA + 1], x - 1, y, z - 1)),
                     lerp(u, grad(permutation[AB + 1], x, y - 1, z - 1),
                          grad(permutation[BB + 1], x - 1, y - 1, z - 1))));
}

// Generate heightmap using Perlin noise
void generateHeightMap(unsigned char heightMap[WIDTH][HEIGHT])
{
    srand(time(NULL));               // Seed the random number generator
    generatePermutation(time(NULL)); // Initialize noise

    float scale = 0.005f;     // Scale for noise (adjust for more/less hills)
    float persistence = 0.1f; // Adjust to control the smoothness of the noise

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            // Generate Perlin noise-based height
            float noiseValue = 0.0f;
            float amplitude = 1.0f;
            float frequency = 1.0f;

            for (int o = 0; o < 4; o++) // 4 octaves for more detail
            {
                noiseValue += PerlinNoise3D(x * frequency * scale, y * frequency * scale, 0.0f) * amplitude;
                amplitude *= persistence;
                frequency *= 2.0f;
            }

            // Normalize the noise value to the height range
            noiseValue = (noiseValue + 1.0f) * 0.5f; // Noise value is now in [0, 1]
            heightMap[x][y] = (unsigned char)(noiseValue * MAX_HEIGHT);
        }
    }
}

void generateHeightMapHill(unsigned char heightMap[WIDTH][HEIGHT])
{
    srand(time(NULL));               // Seed the random number generator
    generatePermutation(time(NULL)); // Initialize noise

    float scale = 0.009f;          // Scale for noise (adjust for more/less hills)
    float persistence = 0.1f;      // Adjust to control the smoothness of the noise
    float islandSizeFactor = 0.5f; // Controls the size of the island (0.5 = medium, 0.7 = large)

    // Center of the map
    float centerX = WIDTH / 2.0f;
    float centerY = HEIGHT / 2.0f;
    float maxDistance = sqrt(centerX * centerX + centerY * centerY) * islandSizeFactor;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            // Calculate distance from the center
            float dx = x - centerX;
            float dy = y - centerY;
            float distance = sqrt(dx * dx + dy * dy);

            // Generate Perlin noise-based height
            float noiseValue = 0.0f;
            float amplitude = 1.0f;
            float frequency = 1.0f;

            for (int o = 0; o < 4; o++) // 4 octaves for more detail
            {
                noiseValue += PerlinNoise3D(x * frequency * scale, y * frequency * scale, 0.0f) * amplitude;
                amplitude *= persistence;
                frequency *= 2.0f;
            }

            // Normalize the noise value to the height range
            noiseValue = (noiseValue + 1.0f) * 0.5f; // Noise value is now in [0, 1]

            // Apply a radial falloff based on distance from the center
            float falloff = 1.0f - (distance / maxDistance);
            falloff = fmax(falloff, 0.0f); // Clamp falloff to [0, 1]

            // Combine noise with falloff to create the island effect
            heightMap[x][y] = (unsigned char)(noiseValue * falloff * MAX_HEIGHT);
        }
    }
}

Model LoadTerrainFromMemory(unsigned char heightMap[WIDTH][HEIGHT], Image sandImage, Image grassImage, Image mountainImage)
{
    // Create an empty image to store the final texture
    Image textureImage = GenImageColor(WIDTH, HEIGHT, WHITE);

    // Set height thresholds for sand, grass, and mountain
    const int sandHeight = MAX_HEIGHT * 0.01;  // Lower threshold for sand
    const int grassHeight = MAX_HEIGHT * 0.02; // Lower threshold for grass

    // Define the texture repetition scale
    const int textureRepeatScale = 1;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            Color color;

            // Get the height value at this point
            int heightValue = heightMap[x][y];

            if (heightValue < sandHeight)
            {
                // Sand texture
                color = GetImageColor(sandImage, (x % (sandImage.width * textureRepeatScale)) / textureRepeatScale,
                                      (y % (sandImage.height * textureRepeatScale)) / textureRepeatScale);
            }
            else if (heightValue < grassHeight)
            {
                // Blend sand and grass textures
                float t = (heightValue - sandHeight) / (float)(grassHeight - sandHeight);
                Color sandColor = GetImageColor(sandImage, (x % (sandImage.width * textureRepeatScale)) / textureRepeatScale,
                                                (y % (sandImage.height * textureRepeatScale)) / textureRepeatScale);
                Color grassColor = GetImageColor(grassImage, (x % (grassImage.width * textureRepeatScale)) / textureRepeatScale,
                                                 (y % (grassImage.height * textureRepeatScale)) / textureRepeatScale);
                color = (Color){
                    (unsigned char)(sandColor.r * (1.0f - t) + grassColor.r * t),
                    (unsigned char)(sandColor.g * (1.0f - t) + grassColor.g * t),
                    (unsigned char)(sandColor.b * (1.0f - t) + grassColor.b * t),
                    255};
            }
            else
            {
                // Blend grass and mountain textures
                float t = (heightValue - grassHeight) / (float)(MAX_HEIGHT - grassHeight);
                Color grassColor = GetImageColor(grassImage, (x % (grassImage.width * textureRepeatScale)) / textureRepeatScale,
                                                 (y % (grassImage.height * textureRepeatScale)) / textureRepeatScale);
                Color mountainColor = GetImageColor(mountainImage, (x % (mountainImage.width * textureRepeatScale)) / textureRepeatScale,
                                                    (y % (mountainImage.height * textureRepeatScale)) / textureRepeatScale);
                color = (Color){
                    (unsigned char)(grassColor.r * (1.0f - t) + mountainColor.r * t),
                    (unsigned char)(grassColor.g * (1.0f - t) + mountainColor.g * t),
                    (unsigned char)(grassColor.b * (1.0f - t) + mountainColor.b * t),
                    255};
            }

            // Set the color in the final texture image
            ImageDrawPixel(&textureImage, x, y, color);
        }
    }

    // Generate a texture from the final texture image
    Texture2D texture = LoadTextureFromImage(textureImage);

    // Generate a 3D mesh from the heightmap image
    Image heightMapImage = {
        .data = heightMap,
        .width = WIDTH,
        .height = HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE};
    Mesh mesh = GenMeshHeightmap(heightMapImage, (Vector3){100, 50, 100});

    // Create a model from the generated mesh
    Model model = LoadModelFromMesh(mesh);

    // Apply the generated texture to the model
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // Unload the images from RAM since they are now on the GPU
    UnloadImage(textureImage);

    // Return the loaded model
    return model;
}

// Modify height at a specific location
void ModifyHeightAt(unsigned char heightMap[WIDTH][HEIGHT], int x, int y, int delta)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        int newHeight = heightMap[x][y] + delta;
        if (newHeight >= 0 && newHeight <= MAX_HEIGHT)
        {
            heightMap[x][y] = (unsigned char)newHeight;
        }
    }
}

// Get the height at a specific location
int GetHeightAt(unsigned char heightMap[WIDTH][HEIGHT], int x, int y)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        return heightMap[x][y];
    }
    return -1; // Invalid height
}

Model GetTerrain(int mapType, Image sandImage, Image grassImage, Image mountainImage)
{

    unsigned char heightMap[WIDTH][HEIGHT];
    switch (mapType)
    {
    case 1:
        generateHeightMapHill(heightMap);
        break;
    default:
        generateHeightMap(heightMap);
        break;
    }

    Model terrain = LoadTerrainFromMemory(heightMap, sandImage, grassImage, mountainImage);

    return terrain;
}