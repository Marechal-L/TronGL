#version 150 core

in vec2 pass_Uv;
in vec3 pass_Color;

uniform sampler2D colormap;
uniform mat4 projection;
uniform mat4 worldView;
uniform mat4 modelWorld;

out vec4 out_Color;

void main()
{
    out_Color = texture(colormap, pass_Uv);
	
    if(out_Color.x > 0.2f && out_Color.y > 0.2f && out_Color.z > 0.2f)
	{
    	out_Color.x += pass_Color.x;
		out_Color.y += pass_Color.y;
		out_Color.z += pass_Color.z;
		
	}
}
