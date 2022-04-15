struct vertex_input
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct vertex_output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

vertex_output main(vertex_input input)
{
    vertex_output output;
    output.pos = input.pos;
    output.color = input.color;
    return output;
}
