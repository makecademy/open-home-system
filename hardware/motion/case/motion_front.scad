// Use common library
use <../../common/primitives.scad>;

// Parameters

// Main box
w = 50;
l = 60;
h = 22;
thickness = 2; 
pcb_attach_h = 12;
draw_box = 1;

// PCB
pcb_width = 51;
pcb_length = 43;
pcb_thickness = 10;
pcb_hole = 1;
pcb_hole_distance = 3.5;
draw_pcb = 0;

// Sensor
sensor_w = 24; 
sensor_l = 24;
sensor_t = 3;
sensor_pcb_w = 24;
sensor_pcb_l = 32;
sensor_pcb_t = 7;
sensor_pcb_hole = 1;
sensor_attach_h = 6;
distance_from_top = 10;
draw_sensor = 0;

// Clips
attach_clip_w = 5;
attach_clip_l = 2;
distance_from_edge = 5;

// Front box
if (draw_box == 1){
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

}

// PCB attaches
pcb_attaches(w,l,h,pcb_length,pcb_width,pcb_hole,pcb_hole_distance,pcb_attach_h);

// PCB
if (draw_pcb == 1){
	pcb(w,l,10,pcb_width,pcb_length,pcb_thickness,pcb_hole,pcb_hole_distance);
}	

// Sensor attaches
rotate([0,-90,0]){
		translate([l-thickness - distance_from_top-sensor_l/2, (w-sensor_pcb_l)/2 + (sensor_pcb_l-sensor_l)/4,-h]) {
			difference(){
				cylinder(h = sensor_attach_h, r=1.5*sensor_pcb_hole, $fn=50);
				translate([0,0,-sensor_attach_h/2]){
					cylinder(h = 20, r=sensor_pcb_hole, $fn=50);
				}
			}
		}

		translate([l-thickness - distance_from_top-sensor_l/2, w - ((w-sensor_pcb_l)/2 + (sensor_pcb_l-sensor_l)/4) ,-h]) {
			difference(){
				cylinder(h = sensor_attach_h, r=1.5*sensor_pcb_hole, $fn=50);
				translate([0,0,-sensor_attach_h/2]){
					cylinder(h = 20, r=sensor_pcb_hole, $fn=50);
				}
			}
		}
}


// Sensor
if (draw_sensor == 1) {
	translate([20,w/2-sensor_w/2,l-thickness-distance_from_top-sensor_l]){
		rotate([0,-90,0]){

			// Sensor body
			cube([sensor_w,sensor_l,sensor_t]);
			translate([0,-(sensor_pcb_l-sensor_l)/2,sensor_t]){

				// Sensor PCB
				difference(){
					cube([sensor_pcb_w,sensor_pcb_l,sensor_pcb_t]);
					translate([sensor_pcb_w/2,(sensor_pcb_l-sensor_l)/4,-5]){
						cylinder(h = 20, r=sensor_pcb_hole, $fn=50);
					}
					translate([sensor_pcb_w/2,sensor_pcb_l - (sensor_pcb_l-sensor_l)/4,-5]){
						cylinder(h = 20, r=sensor_pcb_hole, $fn=50);
					}
				}
				
			}
		}
	}
}