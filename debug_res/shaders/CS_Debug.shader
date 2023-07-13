#version 450
#pragma PROGRAM_COMPUTE

layout(rgba8, set = 4) uniform image2D _MainTex;

shared float pk_ModTime;

layout(local_size_x = 16, local_size_y = 4, local_size_z = 1) in;
void main()
{
    if (gl_LocalInvocationIndex == 0)
    {
        pk_ModTime = mod(float(gl_WorkGroupID.x), 10.0f);
    }

    barrier();

    int2 coord = int2(gl_GlobalInvocationID.xy);
    int2 size = imageSize(_MainTex).xy;
    float4 value = float4(float2(coord + 0.5f.xx) / float2(size) * pk_ModTime * 10.0f, 1.0f, 1.0f);
    imageStore(_MainTex, coord, value);
}