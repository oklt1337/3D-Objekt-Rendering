struct pixel_input
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(pixel_input input) : SV_TARGET
{
    return input.color;
}
