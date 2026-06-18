/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-16T22:14:41-0400
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[74] = {
  0xbea837aa3f0598b5U, 0xbe8eb2693f13decbU, 0x3f2d01f0be0ada62U, 0x3d1f6a29bd8b70d0U,
  0xbd1cae713d652ae9U, 0xbc214161bd296b36U, 0x3d9f6420bf012311U, 0x3ed29a1d3c8613e8U,
  0xbea871933ef01d57U, 0x3dc1262fbd479bb1U, 0xbe5a71ea3ee7a57fU, 0xbe9a79ce3d8332fbU,
  0x3d5f6686bef479feU, 0x3e5f182fbe51921dU, 0xbf110b03beeabb59U, 0xbdf969c8bead940aU,
  0x3eebab473e84cb2fU, 0xbe22cc93beb48502U, 0x3f06f83fbec2cfc4U, 0x3ec0e6333ccbab74U,
  0x3cbec41fbee3bcc5U, 0xbf42d6053f00a873U, 0xbd1f59b8bd0e1301U, 0xbd442dac3d3e6ca3U,
  0x3e341027bc82166fU, 0x3e804c2abdafab88U, 0x3eb7183b3d5d7090U, 0x3ddb9490be3f1cb7U,
  0x3e8831fdbcf414e1U, 0xbc847ce43e4431feU, 0xbce5ddf3be091f49U, 0x3e945b72bd8bfbacU,
  0x3ed2dbc43eec95bfU, 0x3e6fe817beaa9b83U, 0xbe76758b3e1ff8b8U, 0xbf0e1db73d9000c3U,
  0x3ca9eff7bd8cfee2U, 0x3ec90bc5be33d800U, 0xbedece20bed78010U, 0xbeeb411abdd4ca2dU,
  0x3d20778abd11e0a3U, 0xbd2695b0bd43d219U, 0x3e5bf15ebf178d88U, 0xbe20c13fbf059e89U,
  0xbef56f023ebd7f96U, 0x3f04b9bebe463d7fU, 0x3ec39dfbbf1c3879U, 0x3eb87e233ec371e0U,
  0xbe9d8ebabdb918f6U, 0x3eae66213cc5c0e3U, 0xbd63c6593e9ae10eU, 0xbe9cb4693e6ec3c5U,
  0xbee6dfd73edf70a3U, 0xb92d269bbeb52399U, 0xbf011dc2be9ba108U, 0x3e683947bbb2c131U,
  0xbe7f7727bdf7fcc7U, 0x3e905e373ea7f395U, 0xbd8dc13dbda7e0b3U, 0xbcdd9f0cbd73630cU,
  0xbf035b723e82db6fU, 0xbefa2664bde425c0U, 0xbead331f3e0511a9U, 0xbf26003d3d69910eU,
  0xbe81994fbea74a5fU, 0x3ebf3c773e84c4eaU, 0x3f11e59a3f07747eU, 0x3f00f96f3e43fd93U,
  0xbd9f6186bd0f49f0U, 0xbf0883d63db839dbU, 0xbeaa7e9abf1a95ecU, 0xbef853eb3ed85c33U,
  0xbe32d630bee3741fU, 0x345716f6U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

