module case(h, w, l, t) {
    difference(){
		cube([h,w,l]);
		translate([-t,t,t]) {
  			cube([h,w-2*t,l-2*t]);
		}
	}
}

module case_hole(edge_distance_w ,edge_distance_l, hole_radius) {
    translate([0,edge_distance_w,edge_distance_l]) {
		rotate([0,90,0]){
			cylinder(h = 30, r=hole_radius, $fn=50);
		} 
	}
}