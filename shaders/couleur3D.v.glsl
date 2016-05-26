#version 150 core

in vec3 in_Position;
in vec3 in_Color;

uniform mat4 projection;
uniform mat4 worldView;
uniform mat4 modelWorld;
uniform float time;

out vec3 pass_Color;

void main(void) {
	gl_Position = projection * worldView * modelWorld * vec4(in_Position, 1.0);
	pass_Color = vec3(in_Color.x,in_Color.y ,in_Color.z );
}

