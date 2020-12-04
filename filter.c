#include "filter.h"

arm_fir_instance_f32 S[3];
static float32_t sysOut[TEST_LENGTH_SAMPLES];
static float32_t firState[3][BLOCK_SIZE + NUM_TAPS - 1];
const float32_t LP[NUM_TAPS] = {
    0.00256801531776202, 0.00272079238218348, 0.00311263620353168, 0.00374640507156293,
    0.00462028580076543, 0.00572773938519825, 0.00705754331217209, 0.00859393090688225,	
    0.0103168259734834,	0.0122021689107346,	0.0142223284595192,	0.0163465913313102,	
    0.0185417202147791,	0.0207725691026615,	0.0230027435588802,	0.0251952924878157,	
    0.0273134171986650,	0.0293211830967557,	0.0311842191922169,	0.0328703907991000,	
    0.0343504313020686,	0.0355985196829822,	0.0365927916088810,	0.0373157732620591,
    0.0377547287119786,	0.0379019134521019,	0.0377547287119786,	0.0373157732620591,	
    0.0365927916088810,	0.0355985196829822,	0.0343504313020686,	0.0328703907991000,	
    0.0311842191922169,	0.0293211830967557,	0.0273134171986650,	0.0251952924878157,	
    0.0230027435588802,	0.0207725691026615,	0.0185417202147791,	0.0163465913313102,	
    0.0142223284595192,	0.0122021689107346,	0.0103168259734834,	0.00859393090688225,
    0.00705754331217209, 0.00572773938519825, 0.00462028580076543, 0.00374640507156293,	
    0.00311263620353168, 0.00272079238218348, 0.00256801531776202
};

const float32_t BP[NUM_TAPS] = {
    0.000620621608814216, -0.00000000000000000394, -0.000796451012009122, -0.00157087465306679,	
    -0.00191878309068729, -0.00133689342712420, 0.000447958154676441, 0.00308859511793667,	
    0.00545364228459965, 0.00593440956061910, 0.00323370153703784, -0.00267280251217238,	
    -0.00993740230304692, -0.0151033047059912, -0.0143884374229974, -0.00562476666513213,	
    0.00996830306559249, 0.0271242847086238, 0.0375973652035103, 0.0328649634017385,	
    0.00758617181046634, -0.0375576178376362, -0.0949430884237762, -0.151642683785884,	
    -0.193240770908240,	0.792359042604057, -0.193240770908240, -0.151642683785884,	
    -0.0949430884237762, -0.0375576178376362, 0.00758617181046634, 0.0328649634017385,	
    0.0375973652035103,	0.0271242847086238,	0.00996830306559249, -0.00562476666513213,	
    -0.0143884374229974, -0.0151033047059912, -0.00993740230304692,	-0.00267280251217238,	
    0.00323370153703784, 0.00593440956061910, 0.00545364228459965, 0.00308859511793667,	
    0.000447958154676441, -0.00133689342712420,	-0.00191878309068729, -0.00157087465306679,	
    -0.000796451012009122, -0.00000000000000000394, 0.000620621608814216
};

const float32_t HP[NUM_TAPS] = {
    -0.00146553393600304, -0.000896412465447255, -0.000230556379561378, 0.000333607263963554,
    0.000392947349527632, -0.000552719835199917, -0.00277234785370304, -0.00591421730618870,
    -0.00884246245434914, -0.00994348628870112, -0.00791342498148829, -0.00271788443501419,	
    0.00380985430341837, 0.00823106688956470, 0.00678277176297141, -0.00268682070642976,	
    -0.0189484711934604, -0.0367337399570701, -0.0478009694010425, -0.0436329901977656,	
    -0.0188892787628527, 0.0257585595090762, 0.0827087220800146, 0.139063678305381,	
    0.180439053223776, 0.195636652462808, 0.180439053223776, 0.139063678305381,	
    0.0827087220800146, 0.0257585595090762, -0.0188892787628527, -0.0436329901977656,	
    -0.0478009694010425, -0.0367337399570701, -0.0189484711934604, -0.00268682070642976,	
    0.00678277176297141, 0.00823106688956470, 0.00380985430341837, -0.00271788443501419,	
    -0.00791342498148829, -0.00994348628870112, -0.00884246245434914, -0.00591421730618870,	
    0.00277234785370304, -0.000552719835199917, 0.000392947349527632, 0.000333607263963554,	
    -0.000230556379561378, -0.000896412465447255, -0.00146553393600304
};

const float32_t BYPASS[NUM_TAPS] = {
    1, -0.000896412465447255, -0.000230556379561378, 0.000333607263963554,
    0.000392947349527632, -0.000552719835199917, -0.00277234785370304, -0.00591421730618870,
    -0.00884246245434914, -0.00994348628870112, -0.00791342498148829, -0.00271788443501419,
    0.00380985430341837, 0.00823106688956470, 0.00678277176297141, -0.00268682070642976,
    -0.0189484711934604, -0.0367337399570701, -0.0478009694010425, -0.0436329901977656,
    -0.0188892787628527, 0.0257585595090762, 0.0827087220800146, 0.139063678305381,
    0.180439053223776, 0.195636652462808, 0.180439053223776, 0.139063678305381,
    0.0827087220800146, 0.0257585595090762, -0.0188892787628527, -0.0436329901977656,
    -0.0478009694010425, -0.0367337399570701, -0.0189484711934604, -0.00268682070642976,
    0.00678277176297141, 0.00823106688956470, 0.00380985430341837, -0.00271788443501419,
    -0.00791342498148829, -0.00994348628870112, -0.00884246245434914, -0.00591421730618870,
    0.00277234785370304, -0.000552719835199917, 0.000392947349527632, 0.000333607263963554,
    -0.000230556379561378, -0.000896412465447255, -0.00146553393600304
};


float32_t *input, *output;

void init_LP(){
    arm_fir_init_f32(&S[0], NUM_TAPS, (float32_t*)&LP[0], &firState[0][0], BLOCK_SIZE);
}

void init_HP(){
    arm_fir_init_f32(&S[1], NUM_TAPS, (float32_t*)&HP[0], &firState[1][0], BLOCK_SIZE);
}

void init_BP(){
    arm_fir_init_f32(&S[2], NUM_TAPS, (float32_t*)&BP[0], &firState[2][0], BLOCK_SIZE);
}

void init_buff(uint32_t *buffer){
    input = (float32_t*)buffer;
    output = sysOut;
    init_LP();
    init_HP();
    init_BP();
}

void apply_filter(filters_t select){
    switch(select){
        case LOW_PASS:
        	  for(int i=0; i < NUM_BLOCKS; i++)
        	  {
        		  arm_fir_f32(&S[0], input + (i * BLOCK_SIZE), output + (i * BLOCK_SIZE), BLOCK_SIZE);
        	  }
            break;
        case HIGH_PASS:
			  for(int i=0; i < NUM_BLOCKS; i++)
			  {
				  arm_fir_f32(&S[1], input + (i * BLOCK_SIZE), output + (i * BLOCK_SIZE), BLOCK_SIZE);
			  }
            break;
        case BAND_PASS:
			  for(int i=0; i < NUM_BLOCKS; i++)
			  {
				  arm_fir_f32(&S[2], input + (i * BLOCK_SIZE), output + (i * BLOCK_SIZE), BLOCK_SIZE);
			  }
            break;
        case DISABLE:
			  for(int i=0; i < NUM_BLOCKS; i++)
			  {
				  output[i] = input[i];
			  }
			break;
        default:
            break;
    }
}


