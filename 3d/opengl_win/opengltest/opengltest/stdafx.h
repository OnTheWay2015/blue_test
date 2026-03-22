#pragma once

namespace TEST_3D {
    struct RGBA 
    {
        float r;
        float g;
        float b;
        float a;
    };

    struct POS
    {
        float x;
        float y;
        float z;
     
    };

    struct UV
    {
        float u;
        float v;
    };

        union uni_POS_UV 
		{
			struct POS_UV
			{
				POS pos;
				UV UV;
			};
			POS_UV v;
			//float ves[sizeof(POS_UV)/sizeof(POS::x)];
			float ves[sizeof(POS_UV)/sizeof(float)];
		};

    struct VEC4_POS_UV
    {
        uni_POS_UV vec4[4];
    };

}

