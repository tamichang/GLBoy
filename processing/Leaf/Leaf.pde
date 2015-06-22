MyLeaf leaf =  new MyLeaf();


void setup() {
  size(800,600);
  colorMode(HSB,360,100,100,100);
  
  background(0,0,100);


}

void draw() {
  translate(width/2,height/2);
  leaf.draw();
}


class MyLeaf {
  
  int leaf_height_point_count = 40;
  int leaf_height = 300;
  int leaf_height_point_span = leaf_height/leaf_height_point_count;
  int leaf_half_width = 100;
  float branch_angle = radians(75);
  int age = 0;
  int point_line_count = 4;
  float[][][] points = new float[point_line_count+1][leaf_height_point_count][2];
  
  float turn_point = leaf_height_point_count/5.0;
  
  
  void culc_points() {
//    line(0,0, leaf_height,0);
    
    
    for(int i=0; i<leaf_height_point_count; i++) {
      int x1 = i*leaf_height_point_span;
      int y1 = 0;
      float[] p0 = {x1,y1};
      points[0][i] = p0;
      for(int l=1; l<=point_line_count; l++) {
        float[] p = {x1+cos(branch_angle)*leaf_half_width_at_point(i)/point_line_count*l,
                      sin(branch_angle)*leaf_half_width_at_point(i)/point_line_count*l};
        points[l][i] = p;
      }
//      line(x1,y1, x1+cos(branch_angle)*leaf_half_width_at_point(i), sin(branch_angle)*leaf_half_width_at_point(i));
//      line(x1,y1, x1+cos(-branch_angle)*leaf_half_width_at_point(i),sin(-branch_angle)*leaf_half_width_at_point(i));
//      point(x1+cos(branch_angle)*leaf_half_width_at_point(i), sin(branch_angle)*leaf_half_width_at_point(i));
//      point(x1+cos(-branch_angle)*leaf_half_width_at_point(i),sin(-branch_angle)*leaf_half_width_at_point(i));
//      point(x1+cos(branch_angle)*leaf_half_width_at_point(i)/2, sin(branch_angle)*leaf_half_width_at_point(i)/2);
//      point(x1+cos(-branch_angle)*leaf_half_width_at_point(i)/2,sin(-branch_angle)*leaf_half_width_at_point(i)/2);
//      point(x1+cos(branch_angle)*leaf_half_width_at_point(i)/4, sin(branch_angle)*leaf_half_width_at_point(i)/4);
//      point(x1+cos(-branch_angle)*leaf_half_width_at_point(i)/4,sin(-branch_angle)*leaf_half_width_at_point(i)/4);
//      point(x1+cos(branch_angle)*leaf_half_width_at_point(i)/4*3, sin(branch_angle)*leaf_half_width_at_point(i)/4*3);
//      point(x1+cos(-branch_angle)*leaf_half_width_at_point(i)/4*3,sin(-branch_angle)*leaf_half_width_at_point(i)/4*3);
    }
  
  }
  
  void draw() {
    strokeWeight(0.5);
    noStroke();
    culc_points();
    beginShape(TRIANGLES);
    for(int l=0; l<point_line_count; l++) {
      for(int i=0; i<leaf_height_point_count-1; i++) {
//        point(points[l][i][0], points[l][i][1]);
//        point(points[l][i][0], -points[l][i][1]);
        float[] p11 = points[l][i];
        float[] p12 = points[l][i+1];
        float[] p21 = points[l+1][i];
        float[] p22 = points[l+1][i+1];
        fill(360/leaf_height_point_count*i,100,100);
        vertex(p11[0], p11[1]);
        vertex(p12[0], p12[1]);
        vertex(p21[0], p21[1]);
        vertex(p12[0], p12[1]);
        vertex(p22[0], p22[1]);
        vertex(p21[0], p21[1]);
        
        vertex(p11[0], -p11[1]);
        vertex(p12[0], -p12[1]);
        vertex(p21[0], -p21[1]);
        vertex(p12[0], -p12[1]);
        vertex(p22[0], -p22[1]);
        vertex(p21[0], -p21[1]);
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
