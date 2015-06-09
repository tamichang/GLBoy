Plane plane = new Plane();

void setup() {
  size(600,480,P3D);
  colorMode(HSB,360,100,100,100);
  stroke(0,0,100);
  
}

void draw() {
  background(0,27,13);
  translate(width/2, height/2);
  rotateY(map(mouseX-width/2,-width/2,width/2,-PI,PI));
  rotateX(map(mouseY-width/2,-width/2,width/2,-PI,PI));
  
  plane.tick();
  plane.draw();
}

class Plane {
  int line_count = 10;
  int point_count = 100;
  float delta_x = 1.0/(float)(point_count/2);
  float delta_line_angle = TWO_PI/line_count;
  int scale = 100;
  PVector[][] xyz = new PVector[line_count][point_count];
  
  Plane() {
    for(int l=0; l<line_count; l++) {
      for(int p=0; p<point_count/2; p++) {
        float x = p*delta_x;
        float xx = x*scale*cos(delta_line_angle*l);
        float z  = x*scale*sin(delta_line_angle*l);
        float y1 = (sqrt(1-pow(x,2))+(float)Math.cbrt(pow(x,2)))*scale;
        xyz[l][p] = new PVector(xx,y1,z);
      }
      for(int p=point_count/2; p<point_count; p++) {
        float x = 1.0-(p-point_count/2)*delta_x;
        float xx = x*scale*cos(delta_line_angle*l);
        float z  = x*scale*sin(delta_line_angle*l);
        float y2 = (-sqrt(1-pow(x,2))+(float)Math.cbrt(pow(x,2)))*scale;
        xyz[l][p] = new PVector(xx,y2,z);  
      }
    }
  }
  
  int head_point = 0;
  int tail_point = 0;
  void tick() {
    if(tail_point==0)
      head_point++;
    if(head_point==point_count-1)
      tail_point++;
    if(head_point==point_count-1 & tail_point==point_count-1) {
      head_point = 0;
      tail_point = 0;
    }
  }
  
  void draw() {
    for(int l=0; l<line_count; l++) {
    for(int p=tail_point; p<head_point; p++) {
      PVector p1 = xyz[l][p];
      PVector p2 = xyz[l][p+1];
      line(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
    }}
  }
}
