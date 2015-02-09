void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	//get our current vertex position so we can modify it
	vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_Position = pos;
	
}