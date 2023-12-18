Optimizations:
1. Only render faces exposed to air
2. Only render the 3 faces that are exposed to the camera (keep a vbo for each vertices? or s
2. Only render meshes that have centroids within the FOV + some margin
3. Try to combine meshes that share faces and similar block types (greedy meshing) (Use naive approach at first and use greedy meshing when there is time to update) 
4. Only load chunks into memory that are rendered or close to being rendered
5. Transparency: create two vao's: one for transparent and one for opaque blocks, render furthest chunks first (sorted), 

Basic scene updating & rendering stages:
1. Handle input / data 
2. Attempt to get chunks around camera
   - If chunk does not exist, queue the creation of the chunk, chunk data, and mesh creation to chunk generation thread
3. Render chunks around camera (only if they exist)
   - Render opaque
   - Render sorted transparent
4. Render entities
5. Render text

How the world generator works:
1. I am going to create a hash table that accepts a chunks X, Y, Z and maps it to the chunk in memory. 
   It is going to use two arrays to store the key value pairs
2. We want to generate in a sphere
3. I am going to create an array, called rendererChunksOffsets, and fill it with offsets (vec3) from the player's chunk
4. I am then going to create an array, chunksToRender, and fill it with chunks that are at the location of the offsets
5. 

TODO:
3. Add lighting offsets by block face (i.e. top face bright, Y face maybe a bit darker?)
4. Rendering chunk using a paging system with no height limit, but starting from y = 0 with bedrock on floor
5. Use frustum culling
6. Occlusion culling
7. Use noise with the terrain
8. Using multiple threads to create chunk, generate mesh for chunk, with the main thread
9. Solve transparency issues
10. Block picking/showing what block face is selected in the world
11. Block breaking and placing
12. Devise a liquids scheme
13. Devise a lighting scheme
14. Ticking system
15. cube map

Long term changes
1. LOD
2. vertex compression -> from 3 to 1 float
3. Ambient occlusion 