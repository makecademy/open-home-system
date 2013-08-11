// Use common library
use <../../common/primitives.scad>;

// Parameters

// Main box
w = 45;
l = 60;
h = 20;
thickness = 2; 

// Sensor
sensor_w = 25.3; 
sensor_l = 25.3;
distance_from_top = 10;

// Clips
attach_clip_w = 5;
attach_clip_l = 2;
distance_from_edge = h/2 - attach_clip_l;

// Front box
difference(){

	case(h, w, l, thickness);

	// Sensor hole
	translate([1,w/2-sensor_w/2,l-thickness-distance_from_top-sensor_l]) {
			cube([50,sensor_w,sensor_l]);
		} 

	// Clips on the sides
	translate([distance_from_edge,-5,l/2]) {
		cube([attach_clip_l,60,attach_clip_w]);
	} 
	
}