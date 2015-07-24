int satellite_count = 1;
Satellite[] satellites = new Satellite[satellite_count];

void setup() {
  size(600,600,P3D);
  colorMode(HSB,360,100,100,100);
  for(int i=0; i<satellite_count; i++) {
    satellites[i] = new Satellite();
  }
  noStroke();
  background(0,0,0);
  sphereDetail(10);
}

void draw() {
  //background(0,0,0);
  translate(width/2, height/2);
  noFill();
  stroke(0,6,95);
  //sphere(200);
  noStroke();
  fill(0,63,94);
  for(int i=0; i<satellite_count; i++) {
    Satellite satellite = satellites[i];
    satellite.draw();
  }
}

PVector star = new PVector(0,0,0);

class Satellite {
  PVector loc = new PVector(200,0,0);
  PVector speed = new PVector(random(-3,3),random(-3,3),random(-3,3));
  color clr = color(random(360),random(100),94);
  void draw() {
    move();
    pushMatrix();
    translate(loc.x, loc.y, loc.z);
    fill(clr);
    sphere(2);
    popMatrix();
  }
  void move() {
    PVector gravity = PVector.sub(star, loc);
    gravity.normalize();
    gravity.mult(1);
    speed.add(gravity);
    loc.add(speed);
  }
}
