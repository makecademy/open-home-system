// Use common library
use <../../common/primitives.scad>;

// Parameters

// Main box
w = 80;
l = 60;
h = 16;
thickness = 2; 
pcb_attach_h = 6;

// Holes
fixation_hole_r = 1.5;
cylinder_resolution = 50;
distance_from_edges = 15;

// PCB
pcb_width = 50;
pcb_length = 50;
pcb_thickness = 10;
pcb_hole = 1;
pcb_hole_distance = 3.5;
relay_pcb_width = 20;
relay_pcb_length = 40;
draw_pcb = 0;

// Jack opening
connector_width = 11.1 - 2;
connector_distance = 2;

// High voltage connectors openings
high_voltage_width = 11;
inter_connectors_distance = 9;

// Attach
attach_clip_h = 1;
attach_clip_l = 2;
attach_l = 2*(1+attach_clip_l);
attach_w = 5;
attach_h = 1;

// Back box
difference(){

	// Main box
	difference(){
		cube([h,w,l]);
		translate([-thickness,thickness,thickness]) {
  			cube([h,w-2*thickness,l-2*thickness]);
		}
	}

	// Hole 1
	translate([0,distance_from_edges+thickness,distance_from_edges+thickness]) {
		rotate([0,90,0]){
			cylinder(h = 30, r=fixation_hole_r, $fn=cylinder_resolution);
		} 
	}

	// Hole 2
	translate([0,w-distance_from_edges-thickness,l-distance_from_edges-thickness]) {
		rotate([0,90,0]){
			cylinder(h = 30, r=fixation_hole_r, $fn=cylinder_resolution);
		} 
	}

	// Jack opening
	translate([-thickness,w-connector_width -connector_distance - thickness ,-connector_width]){
		cube([h-thickness,connector_width,connector_width*2]);	
	}

	// High voltage openings
	translate([-thickness,-high_voltage_width,(l - 2*high_voltage_width - inter_connectors_distance)/2]){
		cube([h-thickness,high_voltage_width*2,high_voltage_width]);
	}

	translate([-thickness,-high_voltage_width,l-high_voltage_width-(l - 2*high_voltage_width - inter_connectors_distance)/2]){
		cube([h-thickness,high_voltage_width*2,high_voltage_width]);
	}

}



// Attach
translate([-attach_l/2,-attach_h,l/2-attach_w/2]) cube([attach_l,attach_h,attach_w]);
translate([-attach_l/2,0,l/2-attach_w/2]) cube([attach_clip_l,attach_clip_h,attach_w]);

translate([-attach_l/2,w,l/2-attach_w/2]) cube([attach_l,attach_h,attach_w]);
translate([-attach_l/2,w-attach_h,l/2-attach_w/2]) cube([attach_clip_l,attach_clip_h,attach_w]);

// PCB attaches
pcb_attach(thickness,w-pcb_width - 2*thickness,h,pcb_length,pcb_width,pcb_hole,pcb_hole_distance,pcb_attach_h);
pcb_attach((l-relay_pcb_length)/2,thickness,h,relay_pcb_width,relay_pcb_length,pcb_hole,pcb_hole_distance,pcb_attach_h);

// PCB
if (draw_pcb == 1){
	pcb(thickness,(l-relay_pcb_length)/2,10,relay_pcb_length,relay_pcb_width,pcb_thickness,pcb_hole,pcb_hole_distance);
	pcb(w-pcb_width - 2*thickness,thickness,10,pcb_width,pcb_length,pcb_thickness,pcb_hole,pcb_hole_distance);
}	