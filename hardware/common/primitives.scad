// Module case
module case(h, w, l, t) {
    difference(){
		cube([h,w,l]);
		translate([-t,t,t]) {
  			cube([h,w-2*t,l-2*t]);
		}
	}
}

// Module holes
module case_hole(edge_distance_w ,edge_distance_l, hole_radius) {
    translate([0,edge_distance_w,edge_distance_l]) {
		rotate([0,90,0]){
			cylinder(h = 30, r=hole_radius, $fn=50);
		} 
	}
}

// PCB
module pcb(case_width,case_length,translation,pcb_width,pcb_length,pcb_thickness,pcb_hole_radius,pcb_hole_distance) 
{		
	translate([translation,(case_width-pcb_length)/2,(case_length-pcb_width)/2]){
		rotate([0,-90,0]){
	difference(){
		cube([pcb_width,pcb_length,pcb_thickness]);
		translate([pcb_hole_distance,pcb_hole_distance,-10]) {
			cylinder(h = 30, r=pcb_hole_radius, $fn=50);
		}
		translate([pcb_width-pcb_hole_distance,pcb_length-pcb_hole_distance,-10]) {
			cylinder(h = 30, r=pcb_hole_radius, $fn=50);
		}
	}
	}
	}
}

// PCB attaches
module pcb_attaches(case_w,case_l,case_h,pcb_length,pcb_width,pcb_hole_radius,pcb_hole_distance,pcb_attach_h){
	rotate([0,-90,0]){
		translate([(case_w-pcb_length)/2 + pcb_hole_radius + pcb_hole_distance, (case_l-pcb_width)/2 + pcb_hole_distance - pcb_hole_radius,-case_h]) {
			difference(){
				cylinder(h = pcb_attach_h, r=2*pcb_hole_radius, $fn=50);
				translate([0,0,pcb_attach_h/2]){
					cylinder(h = 10, r=pcb_hole_radius, $fn=50);
				}
			}
		}

		translate([case_l-pcb_hole_distance - (case_w-pcb_length)/2 - pcb_hole_radius,case_w-pcb_hole_distance-(case_l-pcb_width)/2 + pcb_hole_radius,-case_h]) {
			difference(){
				cylinder(h = pcb_attach_h, r=2*pcb_hole_radius, $fn=50);
				translate([0,0,pcb_attach_h/2]){
					cylinder(h = 10, r=pcb_hole_radius, $fn=50);
				}
			}
		}
	}
}

// Batteries
module batteries(w,l,translation,battery_r, battery_h, number_batteries){
	for ( i = [0 : number_batteries - 1] ){
		translate([translation,battery_r + (w-6*battery_r)/2+i*battery_r*2,(l-battery_h)/2]){
			cylinder(h = battery_h, r=battery_r, $fn=50);
		}
	}
}