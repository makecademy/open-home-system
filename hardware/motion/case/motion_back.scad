// Use common library
use <../../common/primitives.scad>;

// Parameters

// Main box
w = 45;
l = 60;
h = 20;
thickness = 2; 

// Holes
fixation_hole_r = 1.5;
distance_from_edges = 15;

// Attach
attach_l = h/2;
attach_w = 5;
attach_h = 1;
attach_clip_h = 1;
attach_clip_l = 2;

// Back box
difference(){

	case(h, w, l, thickness);

	case_hole(distance_from_edges,distance_from_edges, fixation_hole_r);
	case_hole(w-distance_from_edges,l-distance_from_edges, fixation_hole_r);

}

// Attach
translate([-attach_l/2,-attach_h,l/2]) cube([attach_l,attach_h,attach_w]);
translate([-attach_l/2,0,l/2]) cube([attach_clip_l,attach_clip_h,attach_w]);

translate([-attach_l/2,w,l/2]) cube([attach_l,attach_h,attach_w]);
translate([-attach_l/2,w-attach_h,l/2]) cube([attach_clip_l,attach_clip_h,attach_w]);