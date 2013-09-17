// Use common library
use <../../common/primitives.scad>;

// Parameters

// Main box
w = 50;
l = 60;
h = 15;
thickness = 2; 
pcb_attach_h = 5;
draw_box = 1;

// PCB
pcb_width = 51;
pcb_length = 43;
pcb_thickness = 10;
pcb_hole = 1;
pcb_hole_distance = 3.5;
draw_pcb = 0;

// Clips
attach_clip_w = 5;
attach_clip_l = 2;
distance_from_edge = 5;

// Front box
if (draw_box == 1){
	difference(){

		case(h, w, l, thickness);

		// Clips on the sides
		translate([distance_from_edge,-5,l/2]) {
			cube([attach_clip_l,60,attach_clip_w]);
		} 
	
	}
}


// PCB attaches
pcb_attaches(w,l,h,pcb_length,pcb_width,pcb_hole,pcb_hole_distance,pcb_attach_h);

// PCB
if (draw_pcb == 1){
	pcb(w,l,10,pcb_width,pcb_length,pcb_thickness,pcb_hole,pcb_hole_distance);
}	