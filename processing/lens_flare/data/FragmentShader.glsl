uniform ivec2 size;

void main()
{
  //vec4 color = gl_FragCoord.y > 300 ? vec4(1,1,1,1) : vec4(0, 0, 1, 0.2);
  vec4 color = vec4(0,0,1,gl_FragCoord.y/size.y);
  gl_FragColor = color;
}