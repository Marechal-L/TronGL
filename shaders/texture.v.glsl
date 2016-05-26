#version 150 core

in vec3 in_Position;
in vec3 in_Color;
in vec2 in_UV;

uniform mat4 projection;
uniform mat4 worldView;
uniform mat4 modelWorld;

out vec2 pass_Uv;

void main()
{
	gl_Position = projection * worldView * modelWorld * vec4(in_Position, 1.0);
    pass_Uv = in_UV;
}
