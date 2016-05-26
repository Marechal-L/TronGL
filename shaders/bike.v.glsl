#version 150 core

in vec3 in_Position;
in vec3 in_Color;
in vec2 in_UV;
in vec3 in_Normal; //!loic: not used, the attrib location won't be valid

uniform mat4 projection;
uniform mat4 worldView;
uniform mat4 modelWorld;

out vec2 pass_Uv;
out vec3 pass_Color;

void main()
{
	gl_Position = projection * worldView * modelWorld * vec4(in_Position, 1.0);
    pass_Uv = in_UV;
	pass_Color = in_Color;
}
