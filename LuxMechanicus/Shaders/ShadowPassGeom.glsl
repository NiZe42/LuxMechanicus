#version 460 core

layout(triangles) in;
// Important - our number of vertices is 3 * lightCount so far
// Glsl limitations - 256, my hardware limitations - 204
// TODO: Change algorithm
layout(triangle_strip, max_vertices = 204) out;

in gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
} gl_in[];

struct ShadowCasterData {
    uint lightIndex;
    int shadowCasterId;
    float pad0;
    float pad1;
    mat4 lightVPMatrix;
};

layout(std430, binding = 2) buffer shadowBuffer {
    ShadowCasterData shadowCasters[];
};

void main()
{
    const int atlasGridSize = 4; // 4x4 tiles per layer
    const float tileSize = 1.0 / float(atlasGridSize);
    for (int i = 0; i < shadowCasters.length(); i++)
    {
        ShadowCasterData data = shadowCasters[i];
        int atlasLayer = data.shadowCasterId / 16;
        int tileIndex = data.shadowCasterId % 16;
        int tileX = tileIndex % 4;
        int tileY = tileIndex / 4;

        mat4 lightVP = data.lightVPMatrix;

        for (int j = 0; j < 3; j++)
        {
            vec4 projected = lightVP * gl_in[j].gl_Position;

            projected.xy = projected.xy * 0.5 + 0.5;

            projected.x = projected.x * tileSize + float(tileX) * tileSize;
            projected.y = projected.y * tileSize + float(tileY) * tileSize;

            projected.xy = projected.xy * 2.0 - 1.0;

            //projected.w = 1.0f;
            //projected.z = 0.0f;
            gl_Position = projected;
            gl_Layer = atlasLayer;
            EmitVertex();
        }
        EndPrimitive();
    }
}
