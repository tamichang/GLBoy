Plane plane = new Plane();
Petals[] petals = new Petals[6];

void setup() {
  size(800,600,P3D);
  colorMode(HSB,360,100,100,100);
  stroke(0,0,100);
  //noStroke();
  //fill(334,45,91);
  noFill();
  
  for(int i=0; i<petals.length; i++) {
    petals[i] = new Petals();
  }
}

void draw() {
  //background(0,27,13);
  background(200,74,25);
  translate(width/2, height/2);
  rotateY(map(mouseX-width/2,-width/2,width/2,-PI,PI));
  rotateX(map(mouseY-width/2,-width/2,width/2,-PI,PI));
  
//  plane.tick();
//  plane.draw();
  
  //petals.tick();
  for(int i=0; i<petals.length; i++) {
    rotateY(TWO_PI/petals.length);
    petals[i].draw();
  }
  
}


class Petals {
  
  int leaf_height_point_count = 50;
  int leaf_height = 300;
  int leaf_height_point_span = leaf_height/leaf_height_point_count;
  float delta_x = 1.0/(float)(leaf_height_point_count/2);
  int scale = 100;
  int leaf_half_width = 100;
  float branch_angle = radians(90);
  int age = 0;
  int point_line_count = 6;
  float[][][] points = new float[point_line_count+1][leaf_height_point_count][3];
  
  float turn_point = leaf_height_point_count/4.0;
  
  void culc_points() {
    float flower_angle = frameCount*0.01;
    
    for(int i=0; i<leaf_height_point_count; i++) {
      float x1;// = i*leaf_height_point_span;
      float y1;// = 0;
      if(i<leaf_height_point_count/2) {
        float x = i*delta_x;
        float y = (sqrt(1-pow(x,2))+(float)Math.cbrt(pow(x,2)));
        y--;
        float xx = x*cos(flower_angle)-y*sin(flower_angle);
        float yy = x*sin(flower_angle)+y*cos(flower_angle);
        yy++;
        x1 = xx * scale;
        y1 = yy * scale;
      } else {
        float x = 1.0-(i-leaf_height_point_count/2)*delta_x;
        float y = (-sqrt(1-pow(x,2))+(float)Math.cbrt(pow(x,2)));
        y--;
        float xx = x*cos(flower_angle)-y*sin(flower_angle);
        float yy = x*sin(flower_angle)+y*cos(flower_angle);
        yy++;
        x1 = xx * scale;
        y1 = yy * scale;
      }
      float[] p0 = {x1,y1,0};
      points[0][i] = p0;
      for(int l=1; l<=point_line_count; l++) {
        //not leaf rolled version
//        float[] p = {x1+cos(branch_angle)*leaf_half_width_at_point(i)/point_line_count*l,
//                      y1,
//                      sin(branch_angle)*leaf_half_width_at_point(i)/point_line_count*l};
        float arc_len = sin(branch_angle)*leaf_half_width_at_point(i)/point_line_count*l;
        float roll_angle = arc_len / x1;
        float [] p = {cos(roll_angle)*x1, y1, sin(roll_angle)*x1};
        points[l][i] = p;
      }
    }
  }
  
  void draw() {
    strokeWeight(0.5);
    noStroke();
    culc_points();
    beginShape(TRIANGLES);
    for(int l=0; l<point_line_count; l++) {
      for(int i=0; i<leaf_height_point_count-1; i++) {
        float[] p11 = points[l][i];
        float[] p12 = points[l][i+1];
        float[] p21 = points[l+1][i];
        float[] p22 = points[l+1][i+1];
        fill(360/leaf_height_point_count*i,100,100);
        vertex(p11[0], p11[1], p11[2]);
        vertex(p12[0], p12[1], p12[2]);
        vertex(p21[0], p21[1], p21[2]);
        vertex(p12[0], p12[1], p12[2]);
        vertex(p22[0], p22[1], p22[2]);
        vertex(p21[0], p21[1], p21[2]);
        
        vertex(p11[0], p11[1], -p11[2]);
        vertex(p12[0], p12[1], -p12[2]);
        vertex(p21[0], p21[1], -p21[2]);
        vertex(p12[0], p12[1], -p12[2]);
        vertex(p22[0], p22[1], -p22[2]);
        vertex(p21[0], p21[1], -p21[2]);
      }
    }
    endShape();
  }
  
  float leave_rate_from_turn_point(int i) {
    if(i < turn_point) {
      return i/turn_point;
    } else {
      return 1 - abs(turn_point - i) / (leaf_height_point_count -1 - turn_point);
    }
    
  }
  
  float leaf_half_width_at_point(int i) {
    return leaf_half_width * sin(map(leave_rate_from_turn_point(i),0,1,0,PI/2));
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


