
WavePlane wp = new WavePlane(800,800);

void setup() {
  size(1200, 1000, P3D);
  colorMode(HSB, 360, 100, 100, 100);
  background(0,0,0);
  stroke(330,13,99);
  //noStroke();
  //fill(150,13,99);
  noFill();
}

void draw() {
  background(212,84,26);
  
//  float camera_distance = (height/2) / tan(PI/6);
//  PVector cameraV = new PVector(mouseX, mouseY, camera_distance);
//  cameraV.sub(new PVector(width/2, height/2, 0));
//  cameraV.normalize();
//  cameraV.mult(camera_distance);
//  //camera(mouseX, mouseY, (height/2) / tan(PI/6), width/2, height/2, 0, 0, 1, 0);
//  camera(cameraV.x, cameraV.y, cameraV.z, width/2, height/2, 0, 0, 1, 0);
  
  
  translate(width/2, height*0.7, 0);
  rotateY(map(mouseX-width/2,-width/2,width/2,-PI,PI));
  rotateX(map(mouseY-width/2,-width/2,width/2,-PI,PI));
  
//  pushMatrix();
//  translate(0,-height*0.1,0);
//  rotateY(frameCount*0.03);
//  box(400);
//  popMatrix();
  
  wp.tick();
  wp.draw();
  
  if(rec) saveFrame("rec/" + System.currentTimeMillis());
}

class WavePlane {
  int w, h;
  
  int line_count = 100;
  int point_count = 100;
  
  float[][][] line = new float[line_count][point_count][3];
  
  float dx, dz;
  int wave_width = 300;
  float da = 0.1;
  float dla = 0.5;
  
  float[][] acnes = new float[1][2]; 
  
  float myu = 0;
  float max_sigma2 = 0.2;
  
  float max_distance = sqrt(pow(abs(0-point_count/2),2) + pow(abs(0-line_count/2),2));
  
  WavePlane(int w, int h) {
    this.w = w; this.h = h;
    dx = w/point_count;
    dz = h/line_count;
  }
  
  void tick() {
    //myu = map(frameCount%60,0,60,7,-7);
    float da = TWO_PI/acnes.length;
    for(int a=0; a<acnes.length; a++) {
      float[] acne = {cos(frameCount*0.03+da*a)*(point_count/4)+point_count/2, sin(frameCount*0.03+da*a)*(line_count/4)+line_count/2};
      acnes[a] = acne;
    }
    
    for(int j=0; j<line_count; j++) {
      //float sigma2 = map(abs(line_count/2-j),0,line_count/2,max_sigma2,5);
      float sigma2 = max_sigma2;
      for(int i=0; i<point_count; i++) {
        float x = i*dx;
        float y = 0;
        for(int a=0; a<acnes.length; a++) {
          //float y = sin(i*da+dla*j+frameCount*0.1) * wave_width/2;
          float distance = sqrt(pow(abs(i-acnes[a][0]),2) + pow(abs(j-acnes[0][1]),2));
          //float y = -1 * 1/sqrt(TWO_PI*sigma2) * exp(-1*pow(map(i,0,point_count-1,-5,5)-myu,2)/(2*sigma2)) * wave_width;
          y += -1 * 1/sqrt(TWO_PI*sigma2) * exp(-1*pow(map(distance,max_distance,0,-5,0)-myu,2)/(2*sigma2)) * wave_width;
          //y = y * map(abs(line_count/2-j),0,line_count/2,2,0);
          //if(frameCount == 10 && j==0) println(map(distance,50,0,-5,0));
        }
        float[] point = {x, y, j*dz};
        line[j][i] = point;
      }
    }
  }
  
  void draw_point() {
    for(int j=0; j<line_count; j++) {
      for(int i=0; i<point_count; i++) {
        float[] p = line[j][i];
        point(p[0], p[1], p[2]);
      }
    }
  }
  
  void draw() {
    pushMatrix();
    translate(-w/2, 0, -h/2);
    for(int l=0; l<line_count-1; l++) {
      for(int p=0; p<point_count-1; p++) {
        float[] p1, p2, p3, p4;
        p1 = line[l][p];
        p2 = line[l][p+1];
        p3 = line[l+1][p];
        p4 = line[l+1][p+1];
        beginShape(TRIANGLES);
        //fill(360/point_count*p,10,95);
        //fill(250,map(p1[1],-50,50,0,100),100);
        vertex(p1[0], p1[1], p1[2]);
        //fill(250,map(p2[1],-50,50,0,100),100);
        vertex(p2[0], p2[1], p2[2]);
        //fill(250,map(p3[1],-50,50,0,100),100);
        vertex(p3[0], p3[1], p3[2]);
        
        //fill(250,map(p2[1],-50,50,0,100),100);
        vertex(p2[0], p2[1], p2[2]);
        //fill(250,map(p3[1],-50,50,0,100),100);
        vertex(p3[0], p3[1], p3[2]);
        //fill(250,map(p4[1],-50,50,0,100),100);
        vertex(p4[0], p4[1], p4[2]);
        endShape();
      }
    }
    for(int a=0; a<acnes.length; a++) {
      pushMatrix();
      translate(dx*acnes[a][1], 0, dz*acnes[a][1]);
      box(30);
      popMatrix();
    }
    popMatrix();
  }
  
}

boolean rec = false;

void keyPressed() {
  if (key == 'r') { 
    rec = rec ^ true;
  }
  if (keyCode == ENTER) {
    saveFrame("image/screen-" + System.currentTimeMillis() + ".png");
  }
}

void mouseWheel(MouseEvent event) {
  float e = event.getCount();
  println(e);
}
