varying vec3 u_color;
uniform mat4 Projection;
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	u_color = gl_Color;
};