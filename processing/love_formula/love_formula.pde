Plane plane = new Plane();
Petals petals = new Petals();

void setup() {
  size(800,600,P3D);
  colorMode(HSB,360,100,100,100);
  stroke(0,0,100);
  noStroke();
  noFill();
}

void draw() {
  //background(0,27,13);
  background(200,74,25);
  translate(width/2, height/2);
  rotateY(map(mouseX-width/2,-width/2,width/2,-PI,PI));
  rotateX(map(mouseY-width/2,-width/2,width/2,-PI,PI));
  
  plane.tick();
  plane.draw();
  
  petals.tick();
  petals.draw();
}

class Petals {
  int line_count = 20;
  int point_count = 50;
  float delta_x = 1.0/(float)(point_count/2);
  float delta_line_angle = TWO_PI/line_count;
  int scale = 100;
  PVector[][] xyz = new PVector[line_count][point_count+1];
  float flower_angle = PI/6;
  
  Petals() {
    
  }
  
  int head_point = 0;
  int tail_point = 0;
  void tick() {
    
    flower_angle = frameCount*0.01;
    
    for(int l=0; l<line_count; l++) {
      for(int p=0; p<point_count/2; p++) {
        float x = p*delta_x;
        float y = (sqrt(1-pow(x,2))+(float)Math.cbrt(pow(x,2)));
        y--;
        float xx = x*cos(flower_angle)-y*sin(flower_angle);
        y        = x*sin(flower_angle)+y*cos(flower_angle);
        y++;
        y *= scale;
        
        float xxx = xx*scale*cos(delta_line_angle*l);
        float z   = xx*scale*sin(delta_line_angle*l);
        
        PVector v = new PVector(xxx,y,z);
        //v.mult(1+noise(l,p,frameCount*0.01));
        xyz[l][p] = v;
      }
      for(int p=point_count/2; p<=point_count; p++) {
        float x = 1.0-(p-point_count/2)*delta_x;
        float y = (-sqrt(1-pow(x,2))+(float)Math.cbrt(pow(x,2)));
        y--;
        float xx = x*cos(flower_angle)-y*sin(flower_angle);
        y        = x*sin(flower_angle)+y*cos(flower_angle);
        y++;
        y *= scale;
        
        float xxx = xx*scale*cos(delta_line_angle*l);
        float z   = xx*scale*sin(delta_line_angle*l);
        
        PVector v = new PVector(xxx,y,z);
        //v.mult(1+noise(l,p,frameCount*0.01));
        xyz[l][p] = v; 
      }
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
    for(int l=0; l<2; l++) {
      //for(int p=tail_point; p<head_point; p++) {
      beginShape(TRIANGLES);
      for(int p=0; p<point_count; p++) {  
        PVector p1 = xyz[l][p];
        PVector p2 = xyz[l][p+1];
        PVector p3 = xyz[(l+1)%xyz.length][p];
        PVector p4 = xyz[(l+1)%xyz.length][p+1];
        fill(100/point_count*p,100,100);
        
        vertex(p1.x, p1.y, p1.z);
        vertex(p2.x, p2.y, p2.z);
        vertex(p3.x, p3.y, p3.z);
        vertex(p2.x, p2.y, p2.z);
        vertex(p4.x, p4.y, p4.z);
        vertex(p3.x, p3.y, p3.z);
        
        
//        if(p < point_count/2) continue;
////        if(p >= point_count/2+1) continue;
//        
//        int l_span = 1;
//        int p_span = 200;
//        PVector p5 = p1;
//        PVector p6 = xyz[(l+l_span*2)%xyz.length][p];
////   ・２///fdsfdsf/   PVector p7 = PVector.add(p5,p6);
//        PVector p7 = xyz[(l+l_span)%xyz.length][min(p+p_span, xyz[0].length-1)];
//        p7 = PVector.mult(p7,(1+20.0/p));
//        
////        p5.mult(1);
////        fill(100/point_count*p+200,10,10);
//        beginShape(TRIANGLES);
//        vertex(p5.x, p5.y, p5.z);
//        vertex(p6.x, p6.y, p6.z);
//        vertex(p7.x, p7.y, p7.z);
//        endShape();
      }
    endShape();
    }
  }
}

class Plane {
  int line_count = 20;
  int point_count = 50;
  float delta_x = 1.0/(float)(point_count/2);
  float delta_line_angle = TWO_PI/line_count;
  int scale = 100;
  PVector[][] xyz = new PVector[line_count][point_count+1];
  
  Plane() {
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
        //v.mult(1+noise(l,p,frameCount*0.01));
        xyz[l][p] = v; 
      }
    }
  }
  
  int head_point = 0;
  int tail_point = 0;
  void tick() {
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
        PVector p3 = xyz[(l+1)%xyz.length][p];
        PVector p4 = xyz[(l+1)%xyz.length][p+1];
        fill(100/point_count*p,100,100);
        beginShape(TRIANGLES);
        vertex(p1.x, p1.y, p1.z);
        vertex(p2.x, p2.y, p2.z);
        vertex(p3.x, p3.y, p3.z);
        vertex(p2.x, p2.y, p2.z);
        vertex(p4.x, p4.y, p4.z);
        vertex(p3.x, p3.y, p3.z);
        endShape();
        
//        if(p < point_count/2) continue;
//        if(p >= point_count/2+1) continue;
//        
//        PVector p5 = p1;
//        PVector p6 = xyz[(l+3)%xyz.length][p];
//        PVector p7 = PVector.add(p5,p6);
//        
////        p5.mult(1);
////        fill(100/point_count*p+200,10,10);
//        beginShape(TRIANGLES);
//        vertex(p5.x, p5.y, p5.z);
//        vertex(p6.x, p6.y, p6.z);
//        vertex(p7.x, p7.y, p7.z);
//        endShape();
      }
    }
  }
}
