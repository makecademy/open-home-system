// Use common library
use <../../common/primitives.scad>;

// Parameters

// Main box
w = 60;
l = 60;
h = 6;
thickness = 2; 
pcb_attach_h = 5;
draw_box = 1;

// Clips
attach_clip_w = 5;
attach_clip_l = 2;
distance_from_edge = 1;

// Front box
if (draw_box == 1){
	difference(){

		case(h, w, l, thickness);

		// Clips on the sides
		translate([distance_from_edge,-5,l/2]) {
			cube([attach_clip_l,100,attach_clip_w]);
		} 
	
	}
}