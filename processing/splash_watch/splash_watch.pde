int interval = 110;
int time_disp = 10;
int seq = 0;
int splash_reverse_time = time_disp + (interval-time_disp)/2;

void setup() {
  size(600,480);
  colorMode(HSB,360,100,100,100);
  PFont font = createFont("7barSP", 32, true);//loadFont("BradleyHandITC-48.vlw");
  textFont(font, 62);
  
  //noStroke();
}

PImage img;
float[][] points;
String time_str;
int[] text_loc = {200, 200};
int text_h = 50;
int text_w;

void draw() {
  fill(131,15,99);
  
  //translate(width/2, height/2);
  
  if(seq == 0) {
    background(0,0,0);
    time_str = hour() + ":" + minute() + ":" + second();
    text_w = ceil(textWidth(time_str));
    text(time_str, text_loc[0], text_loc[1]+text_h);
    updateImage(); 
  } else if(seq < time_disp) {
    background(0,0,0);
    text(time_str, text_loc[0], text_loc[1]+text_h);
    //rect(0,0,text_loc[0], text_loc[1]+text_h);
  } else {
    float alpha = 50.0/interval*(seq-10);
    fill(0,0,0,alpha);
    //background(0,2,100);
    noStroke();
    rect(0,0,width,height);
    stroke(131,15,99,100-alpha);
    //text(time_str, text_loc[0], text_loc[1]);
    splash();
    //image(img,text_loc[0], text_loc[1]);
    //img.updatePixels();
    //rect(text_loc[0], text_loc[1] - text_h, text_w, text_h);
  }
  //rect(text_loc[0], text_loc[1] - text_h, text_w, text_h);
  seq = (seq+1) % interval;
  //image(img,0,0);
}

void updateImage() {
  img = get(text_loc[0], text_loc[1], text_w, text_h);
  //rect(text_loc[0], text_loc[1] - text_h, text_w, text_h);
  points = new float[img.pixels.length][2];
  int cursor = 0;
  color bc = img.pixels[0];
  for(int i=0; i<img.pixels.length; i++) {
    if(img.pixels[i] != bc) {
      float[] point = {i%text_w, i/text_w};
      points[cursor] = point;
      cursor++;
    }
  }
}

void splash() {
  //rect(text_loc[0], text_loc[1] - text_h, text_w, text_h);
//  for(int i=0; i<img.pixels.length; i++) {
//    color c = img.pixels[i];
//    img.pixels[i] = floor(c*noise(i,frameCount*0.01)*2);
//  }
  int i =0;
  float[] p = points[i];
  int speed = 6;
  pushMatrix();
  translate(text_loc[0], text_loc[1]);
  int n = seq < splash_reverse_time ? seq : splash_reverse_time - (seq - splash_reverse_time);
  int minus = seq < splash_reverse_time ? 1 : -1;
  while(p[0] != 0.0 & p[1] != 0.0) {
    //if(seq < splash_reverse_time) {
      p[0] += minus*cos(noise(i,n*0.03)*PI/2)*speed;
      p[1] += minus*sin(map(noise(i,n*0.2),0,1,-PI/3, PI/6))*speed;
    //} else {
      
    //  p[0] -= cos(noise(i,reverse_seq*0.03)*PI/2)*speed;
    //  p[1] -= sin(map(noise(i,reverse_seq*0.2),0,1,-PI/3, PI/6))*speed;
    //}
    point(p[0], p[1]);
//    println(p);
    i++;
    p = points[i];
  }
  popMatrix();
}
