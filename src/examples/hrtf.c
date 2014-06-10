/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/

/**Demonstrates the hrtf node.*/
#include <libaudioverse/private_all.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERRCHECK(x) do {\
if((x) != Lav_ERROR_NONE) {\
	printf(#x " errored: %i", (x));\
	return;\
}\
} while(0)\

void main(int argc, char** args) {
	if(argc != 3) {
		printf("Usage: %s <sound file> <hrtf file>", args[0]);
		return;
	}
	LavDevice* device;
	LavObject* fileNode, *hrtfNode;
	ERRCHECK(Lav_initializeLibrary());
	ERRCHECK(Lav_createDefaultAudioOutputDevice(&device));
	ERRCHECK(Lav_createFileNode(device, args[1], &fileNode));

	LavHrtfData *hrtf = NULL;
	ERRCHECK(Lav_createHrtfData(args[2], &hrtf));
	ERRCHECK(Lav_createHrtfNode(device, hrtf, &hrtfNode));

	ERRCHECK(Lav_setParent(hrtfNode, fileNode, 0, 0));
	ERRCHECK(Lav_deviceSetOutputObject(device, hrtfNode));
	int shouldContinue = 1;
	printf("Enter pairs of numbers separated by whitespace, where the first is azimuth (anything) and the second\n"
"is elevation (-90 to 90).\n"
"Input q to quit.\n");
	char command[512] = "";
	float elev = 0, az = 0;
	int elevOrAz = 0;
	while(shouldContinue) {
		scanf("%s", command);
		if(command[0] == 'q') {
			shouldContinue = 0;
			continue;
		}
		if(elevOrAz == 0) {
			sscanf(command, "%f", &az);
			elevOrAz = 1;
			continue;
		}
		else if(elevOrAz == 1) {
			sscanf(command, "%f", &elev);
			ERRCHECK(Lav_setFloatProperty(hrtfNode, Lav_HRTF_ELEVATION, elev));
			ERRCHECK(Lav_setFloatProperty(hrtfNode, Lav_HRTF_AZIMUTH, az));
			elevOrAz = 0;
			continue;
		}
	}
}
