// Parameters

// Main box
w = 110;
l = 164;
h = 30;
thickness = 2; 

// Holes
fixation_hole_r = 2.9;
cylinder_resolution = 50;
distance_from_edges = 20;

// Mounting Raspberry Pi
mount_h = 10;
mount_r = 5;
rpi_from_edges = 25;
rpi_w = 56;
rpi_l = 85;
h_1_w = 12.5;
h_1_l = 5;
h_2_w = 18;
h_2_l = 25.5;


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
			cylinder(h = 40, r=fixation_hole_r, $fn=cylinder_resolution);
		} 
	}

	translate([0,w-distance_from_edges-thickness,l-distance_from_edges-thickness]) {
		rotate([0,90,0]){
			cylinder(h = 40, r=fixation_hole_r, $fn=cylinder_resolution);
		} 
	}

}

difference(){
translate([h-mount_h,h_1_w+rpi_from_edges+thickness,h_1_l+thickness+rpi_from_edges]) {
		rotate([0,90,0]){
			cylinder(h = mount_h, r=mount_r, $fn=cylinder_resolution);
		} 
	}

translate([h-mount_h-2*thickness,h_1_w+rpi_from_edges+thickness,h_1_l+thickness+rpi_from_edges]) {
		rotate([0,90,0]){
			cylinder(h = mount_h, r=fixation_hole_r, $fn=cylinder_resolution);
		} 
	}
}

difference(){
translate([h-mount_h-thickness,rpi_w+rpi_from_edges+thickness-h_2_w,rpi_l+thickness+rpi_from_edges-h_2_l]) {
		rotate([0,90,0]){
			cylinder(h = mount_h, r=mount_r, $fn=cylinder_resolution);
		} 
	}

translate([h-mount_h-2*thickness,rpi_w+rpi_from_edges+thickness-h_2_w,rpi_l+thickness+rpi_from_edges-h_2_l]) {
		rotate([0,90,0]){
			cylinder(h = mount_h, r=fixation_hole_r, $fn=cylinder_resolution);
		} 
	}
}