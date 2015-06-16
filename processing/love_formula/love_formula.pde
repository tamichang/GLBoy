Plane plane = new Plane();

void setup() {
  size(1000,780,P3D);
  colorMode(HSB,360,100,100,100);
  stroke(0,0,100);
  noStroke();
  noFill();
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
  int line_count = 20;
  int point_count = 50;
  float delta_x = 1.0/(float)(point_count/2);
  float delta_line_angle = TWO_PI/line_count;
  int scale = 100;
  PVector[][] xyz = new PVector[line_count+1][point_count+1];
  
  Plane() {
  }
  
  int head_point = 0;
  int tail_point = 0;
  void tick() {
    for(int l=0; l<line_count; l++) {
      for(int p=0; p<point_count/2; p++) {
        float x = p*delta_x;
        float xx = x*scale*cos(delta_line_angle*l);
        float z  = x*scale*sin(delta_line_angle*l);
        float y1 = (sqrt(1-pow(x,2))+(float)Math.cbrt(pow(x,2)))*scale;
        PVector v = new PVector(xx,y1,z);
        //v.mult(1+noise(l,p,frameCount*0.01));
        xyz[l][p] = v;
      }
      for(int p=point_count/2; p<=point_count; p++) {
        float x = 1.0-(p-point_count/2)*delta_x;
        float xx = x*scale*cos(delta_line_angle*l);
        float z  = x*scale*sin(delta_line_angle*l);
        float y2 = (-sqrt(1-pow(x,2))+(float)Math.cbrt(pow(x,2)))*scale;
        PVector v = new PVector(xx,y2,z);
        v.mult(1+noise(l,p,frameCount*0.01));
        xyz[l][p] = v; 
      }
    }
    for(int p=0; p<=point_count; p++) {
      xyz[line_count][p] = xyz[0][p];
    }
    
    if(tail_point==0)
      head_point++;
    if(head_point==point_count)
      tail_point++;
    if(head_point==point_count & tail_point==point_count) {
      head_point = 0;
      tail_point = 0;
    }
  }
  
  void draw() {
    for(int l=0; l<line_count; l++) {
      //for(int p=tail_point; p<head_point; p++) {
      for(int p=0; p<point_count; p++) {  
        PVector p1 = xyz[l][p];
        PVector p2 = xyz[l][p+1];
        PVector p3 = xyz[l+1][p];
        PVector p4 = xyz[l+1][p+1];
        fill(100/point_count*p,100,100);
        beginShape(TRIANGLES);
        vertex(p1.x, p1.y, p1.z);
        vertex(p2.x, p2.y, p2.z);
        vertex(p3.x, p3.y, p3.z);
        vertex(p2.x, p2.y, p2.z);
        vertex(p4.x, p4.y, p4.z);
        vertex(p3.x, p3.y, p3.z);
        endShape();
      }
    }
  }
}
