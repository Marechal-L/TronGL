#version 150 core

in vec2 pass_Uv;

uniform sampler2D colormap;
uniform mat4 projection;
uniform mat4 worldView;
uniform mat4 modelWorld;
uniform float time;

out vec4 out_Color;

void main()
{
    out_Color = texture(colormap, pass_Uv);
    if(out_Color.z > 0.5f)
		out_Color.z *= abs(sin(time));
}
