MyLeaf leaf =  new MyLeaf();


void setup() {
  size(800,600);
  colorMode(HSB,360,100,100,100);
  
  background(0,0,100);
  translate(width/2,height/2);
  leaf.draw();
}


class MyLeaf {
  
  int leaf_height_point_count = 30;
  int leaf_height = 300;
  int leaf_height_point_span = leaf_height/leaf_height_point_count;
  int leaf_half_width = 100;
  float branch_angle = radians(70);
  float reduction_rate = 0.8;
  int age = 0;
  int point_line_count = 4;
  float[][] points = new float[point_line_count+1][2];
  
  float turn_point = leaf_height_point_count/5.0;
  
  
  void culc_points() {
//    line(0,0, leaf_height,0);
    
    
    for(int i=0; i<leaf_height_point_count; i++) {
      int x1 = i*leaf_height_point_span;
      int y1 = 0;
      float[] p0 = {x1,y1};
      points[0] = p0;
      for(int l=1; l<=point_line_count; l++) {
        float[] p = {x1+cos(branch_angle)*leaf_half_width_at_point(i)/point_line_count*l,
                      sin(branch_angle)*leaf_half_width_at_point(i)/point_line_count*l};
        points[l] = p;
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
  
  float leave_rate_from_turn_point(int i) {
    if(i < turn_point) {
      return i/turn_point;
    } else {
      return 1 - abs(turn_point - i) / (leaf_height_point_count - turn_point);
    }
    
  }
  
  float leaf_half_width_at_point(int i) {
    return leaf_half_width * sin(map(leave_rate_from_turn_point(i),0,1,0,PI/2));
  }

}
