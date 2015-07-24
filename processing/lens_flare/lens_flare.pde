PShader sd; 

void setup() {
  size(600,600,P2D);
  colorMode(HSB,360,100,100,100);
  sd = loadShader("FragmentShader.glsl");
  sd.set("size", width, height);
}

void draw() {
  shader(sd);
  background(0,0,0);
  //translate(width/2, height/2);
  //fill(93,30,95,20);
  ellipse(mouseX,mouseY,300,300);
  //rect(0,0,width,height);
}
