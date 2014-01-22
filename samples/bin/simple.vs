attribute vec4 vPosition;
void main()
{
	gl_Position = vPosition;
	gl_Position.w = 100.0;
}
