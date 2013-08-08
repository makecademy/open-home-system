// Parameters

// Main box
w = 45;
l = 60;
h = 20;
thickness = 2; 

// Holes
fixation_hole_r = 1.5;
cylinder_resolution = 50;
distance_from_edges = 15;

// Attach
attach_l = h/2;
attach_w = 5;
attach_h = 1;
attach_clip_h = 1;
attach_clip_l = 2;

// Back box
difference(){

	difference(){
		cube([h,w,l]);
		translate([-thickness,thickness,thickness]) {
  			cube([h,w-2*thickness,l-2*thickness]);
		}
	}

	translate([0,distance_from_edges+thickness,distance_from_edges+thickness]) {
		rotate([0,90,0]){
			cylinder(h = 30, r=fixation_hole_r, $fn=cylinder_resolution);
		} 
	}

	translate([0,w-distance_from_edges-thickness,l-distance_from_edges-thickness]) {
		rotate([0,90,0]){
			cylinder(h = 30, r=fixation_hole_r, $fn=cylinder_resolution);
		} 
	}

}

// Attach
translate([-attach_l/2,-attach_h,l/2]) cube([attach_l,attach_h,attach_w]);
translate([-attach_l/2,0,l/2]) cube([attach_clip_l,attach_clip_h,attach_w]);

translate([-attach_l/2,w,l/2]) cube([attach_l,attach_h,attach_w]);
translate([-attach_l/2,w-attach_h,l/2]) cube([attach_clip_l,attach_clip_h,attach_w]);