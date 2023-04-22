#pragma once
#include "paddle_lite_factory_helper.h"

USE_LITE_KERNEL(layer_norm, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(squeeze, kHost, kAny, kAny, def);
USE_LITE_KERNEL(squeeze2, kHost, kAny, kAny, def);
USE_LITE_KERNEL(expand, kHost, kAny, kAny, def);
USE_LITE_KERNEL(reduce_all, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(gru_unit, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(conv2d, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(depthwise_conv2d, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(conv2d, kARM, kInt8, kNCHW, int8_out);
USE_LITE_KERNEL(conv2d, kARM, kInt8, kNCHW, fp32_out);
USE_LITE_KERNEL(depthwise_conv2d, kARM, kInt8, kNCHW, int8_out);
USE_LITE_KERNEL(depthwise_conv2d, kARM, kInt8, kNCHW, fp32_out);
USE_LITE_KERNEL(lstm, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(lstm, kARM, kInt8, kNCHW, def);
USE_LITE_KERNEL(lod_reset, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(anchor_generator, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(slice, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(slice, kARM, kFloat, kNCHW, bool_slice);
USE_LITE_KERNEL(slice, kARM, kFloat, kNCHW, int32_slice);
USE_LITE_KERNEL(slice, kARM, kFloat, kNCHW, def_int64);
USE_LITE_KERNEL(im2sequence, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(meshgrid, kHost, kFloat, kAny, float32);
USE_LITE_KERNEL(meshgrid, kHost, kFloat, kAny, int32);
USE_LITE_KERNEL(fc, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(fc, kARM, kInt8, kNCHW, int8out);
USE_LITE_KERNEL(fc, kARM, kInt8, kNCHW, fp32out);
USE_LITE_KERNEL(pad2d, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(scale, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(scale, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(scale, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(reduce_sum, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(lod_array_length, kHost, kAny, kAny, def);
USE_LITE_KERNEL(unbind, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(unbind, kHost, kInt64, kNCHW, def);
USE_LITE_KERNEL(select_input, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(pool2d, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(retinanet_detection_output, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(decode_bboxes, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(sequence_expand_as, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(where_index, kHost, kAny, kAny, def);
USE_LITE_KERNEL(split, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(split, kHost, kFloat, kNCHW, int32);
USE_LITE_KERNEL(split, kHost, kFloat, kNCHW, int64);
USE_LITE_KERNEL(split, kHost, kInt64, kNCHW, def);
USE_LITE_KERNEL(increment, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(fill_constant_batch_size_like, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(argsort, kHost, kFloat, kAny, argsort_fp32);
USE_LITE_KERNEL(argsort, kHost, kFloat, kAny, argsort_int32);
USE_LITE_KERNEL(argsort, kHost, kFloat, kAny, argsort_int64);
USE_LITE_KERNEL(distribute_fpn_proposals, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(assign_value, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(fill_constant, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(lod_reset, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(assign, kHost, kAny, kAny, def);
USE_LITE_KERNEL(assign, kHost, kAny, kAny, def_tensor_array);
USE_LITE_KERNEL(matrix_nms, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(split, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(split, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(tensor_array_to_tensor, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(tile, kHost, kFloat, kNCHW, def_float);
USE_LITE_KERNEL(tile, kHost, kInt32, kNCHW, def_int32);
USE_LITE_KERNEL(tile, kHost, kInt64, kNCHW, def_int64);
USE_LITE_KERNEL(tile, kHost, kInt8, kNCHW, def_int8);
USE_LITE_KERNEL(tile, kHost, kFloat, kNCHW, def_bool);
USE_LITE_KERNEL(layout, kARM, kFloat, kNCHW, nchw2nhwc);
USE_LITE_KERNEL(layout, kARM, kFloat, kNCHW, nhwc2nchw);
USE_LITE_KERNEL(layout, kARM, kInt8, kNCHW, int8_nchw2nhwc);
USE_LITE_KERNEL(layout, kARM, kInt8, kNCHW, int8_nhwc2nchw);
USE_LITE_KERNEL(layout_once, kARM, kFloat, kNCHW, nchw2nhwc);
USE_LITE_KERNEL(layout_once, kARM, kFloat, kNCHW, nhwc2nchw);
USE_LITE_KERNEL(layout_once, kARM, kInt8, kNCHW, int8_nchw2nhwc);
USE_LITE_KERNEL(layout_once, kARM, kInt8, kNCHW, int8_nhwc2nchw);
USE_LITE_KERNEL(beam_search, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(relu, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(leaky_relu, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(prelu, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(sigmoid, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(tanh, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(relu6, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(thresholded_relu, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elu, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(top_k, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(affine_channel, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(sequence_unpad, kHost, kFloat, kAny, float32);
USE_LITE_KERNEL(sequence_unpad, kHost, kFloat, kAny, int64);
USE_LITE_KERNEL(crop_tensor, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(crop_tensor, kHost, kFloat, kAny, int32_precision);
USE_LITE_KERNEL(uniform_random, kHost, kAny, kAny, def);
USE_LITE_KERNEL(sequence_mask, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(sequence_mask, kHost, kFloat, kNCHW, int32);
USE_LITE_KERNEL(sequence_mask, kHost, kFloat, kNCHW, int64);
USE_LITE_KERNEL(reduce_mean, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(stack, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(stack, kHost, kFloat, kAny, int32_def);
USE_LITE_KERNEL(stack, kHost, kFloat, kAny, int64_def);
USE_LITE_KERNEL(generate_proposals, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(gather, kARM, kFloat, kNCHW, int32int32);
USE_LITE_KERNEL(gather, kARM, kFloat, kNCHW, int64int64);
USE_LITE_KERNEL(gather, kARM, kFloat, kNCHW, int64int32);
USE_LITE_KERNEL(gather, kARM, kFloat, kNCHW, int32int64);
USE_LITE_KERNEL(arg_max, kARM, kAny, kNCHW, fp32);
USE_LITE_KERNEL(arg_max, kARM, kAny, kNCHW, int64);
USE_LITE_KERNEL(arg_max, kARM, kAny, kNCHW, int32);
USE_LITE_KERNEL(arg_max, kARM, kAny, kNCHW, int16);
USE_LITE_KERNEL(arg_max, kARM, kAny, kNCHW, uint8);
USE_LITE_KERNEL(group_norm, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(crop, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(crop, kHost, kInt32, kAny, def);
USE_LITE_KERNEL(bilinear_interp, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(nearest_interp, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(bilinear_interp_v2, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(nearest_interp_v2, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(expand_as, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(expand_as, kHost, kFloat, kAny, int64);
USE_LITE_KERNEL(fetch, kHost, kAny, kAny, def);
USE_LITE_KERNEL(transpose, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(transpose2, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(prior_box, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(pixel_shuffle, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(collect_fpn_proposals, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(reduce_prod, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(reduce_prod, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(roi_align, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(pixel_shuffle, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(softmax, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(gru, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(gru, kARM, kInt8, kNCHW, def);
USE_LITE_KERNEL(ctc_align, kHost, kInt64, kNCHW, def);
USE_LITE_KERNEL(ctc_align, kHost, kInt32, kNCHW, def);
USE_LITE_KERNEL(yolo_box, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(matmul, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(matmul, kARM, kInt8, kNCHW, def);
USE_LITE_KERNEL(density_prior_box, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(assign_value, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(unsqueeze, kHost, kAny, kAny, def);
USE_LITE_KERNEL(unsqueeze2, kHost, kAny, kAny, def);
USE_LITE_KERNEL(top_k, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(flatten_contiguous_range, kHost, kAny, kAny, def);
USE_LITE_KERNEL(linspace, kHost, kFloat, kAny, float32);
USE_LITE_KERNEL(linspace, kHost, kInt32, kAny, int32);
USE_LITE_KERNEL(one_hot, kHost, kAny, kAny, def);
USE_LITE_KERNEL(one_hot_v2, kHost, kAny, kAny, def);
USE_LITE_KERNEL(one_hot_v2, kHost, kAny, kAny, one_hot_v2_int32);
USE_LITE_KERNEL(distribute_fpn_proposals, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(shape, kHost, kAny, kAny, def);
USE_LITE_KERNEL(reshape, kHost, kAny, kAny, def);
USE_LITE_KERNEL(reshape2, kHost, kAny, kAny, def);
USE_LITE_KERNEL(flatten, kHost, kAny, kAny, def);
USE_LITE_KERNEL(flatten2, kHost, kAny, kAny, def);
USE_LITE_KERNEL(sequence_softmax, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(strided_slice, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(strided_slice, kHost, kInt32, kNCHW, def);
USE_LITE_KERNEL(strided_slice, kHost, kInt64, kNCHW, def);
USE_LITE_KERNEL(box_coder, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(lookup_table, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(lookup_table_v2, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(correlation, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(generate_proposals_v2, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(rnn, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(relu_clipped, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(swish, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(log, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(exp, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(floor, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(hard_sigmoid, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(sqrt, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(rsqrt, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(square, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(hard_swish, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(reciprocal, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(abs, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(gelu, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(equal, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(equal, kHost, kInt64, kAny, def);
USE_LITE_KERNEL(equal, kHost, kFloat, kAny, int64);
USE_LITE_KERNEL(equal, kHost, kInt32, kAny, def);
USE_LITE_KERNEL(equal, kHost, kFloat, kAny, int32);
USE_LITE_KERNEL(not_equal, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(not_equal, kHost, kFloat, kAny, int32);
USE_LITE_KERNEL(not_equal, kHost, kFloat, kAny, int64);
USE_LITE_KERNEL(less_than, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(less_than, kHost, kInt32, kAny, def);
USE_LITE_KERNEL(less_than, kHost, kFloat, kAny, int32);
USE_LITE_KERNEL(less_than, kHost, kInt64, kAny, def);
USE_LITE_KERNEL(less_than, kHost, kFloat, kAny, int64);
USE_LITE_KERNEL(less_equal, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(less_equal, kHost, kInt64, kAny, def);
USE_LITE_KERNEL(greater_than, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(greater_than, kHost, kInt64, kAny, def);
USE_LITE_KERNEL(greater_than, kHost, kInt32, kAny, def);
USE_LITE_KERNEL(greater_equal, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(greater_equal, kHost, kInt64, kAny, def);
USE_LITE_KERNEL(negative, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(mean, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(range, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(range, kHost, kInt64, kAny, def);
USE_LITE_KERNEL(range, kHost, kInt32, kAny, def);
USE_LITE_KERNEL(range, kHost, kFloat, kAny, int32);
USE_LITE_KERNEL(range, kHost, kFloat, kAny, int64);
USE_LITE_KERNEL(conditional_block, kHost, kAny, kAny, def);
USE_LITE_KERNEL(cos, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(calib, kARM, kInt8, kNCHW, fp32_to_int8);
USE_LITE_KERNEL(calib, kARM, kInt32, kNCHW, int32_to_fp32);
USE_LITE_KERNEL(calib, kARM, kInt32, kNCHW, int32_to_int64);
USE_LITE_KERNEL(calib, kARM, kInt32, kNCHW, fp32_to_int32);
USE_LITE_KERNEL(calib, kARM, kInt64, kNCHW, int64_to_fp32);
USE_LITE_KERNEL(calib, kARM, kInt8, kNCHW, int8_to_fp32);
USE_LITE_KERNEL(calib, kARM, kInt8, kNHWC, fp32_to_int8);
USE_LITE_KERNEL(calib, kARM, kInt8, kNHWC, int8_to_fp32);
USE_LITE_KERNEL(calib, kARM, kInt64, kNCHW, int64_to_int32);
USE_LITE_KERNEL(calib_once, kARM, kInt8, kNCHW, fp32_to_int8);
USE_LITE_KERNEL(calib_once, kARM, kInt8, kNCHW, int8_to_fp32);
USE_LITE_KERNEL(calib_once, kARM, kInt8, kNHWC, fp32_to_int8);
USE_LITE_KERNEL(calib_once, kARM, kInt8, kNHWC, int8_to_fp32);
USE_LITE_KERNEL(calib_once, kARM, kInt64, kNCHW, int64_to_int32);
USE_LITE_KERNEL(pad3d, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(shuffle_channel, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(stack, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(stack, kARM, kFloat, kNCHW, int32_def);
USE_LITE_KERNEL(scatter, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(crf_decoding, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(instance_norm, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(feed, kHost, kAny, kAny, def);
USE_LITE_KERNEL(sin, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(gather_tree, kHost, kFloat, kAny, int32);
USE_LITE_KERNEL(gather_tree, kHost, kFloat, kAny, int64);
USE_LITE_KERNEL(reduce_max, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(norm, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(p_norm, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(merge_lod_tensor, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(conv2d_transpose, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(conv2d_transpose, kARM, kInt8, kNCHW, fp32_out);
USE_LITE_KERNEL(conv2d_transpose, kARM, kInt8, kNCHW, int8_out);
USE_LITE_KERNEL(split_lod_tensor, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(expand_v2, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(expand_v2, kHost, kInt32, kAny, def);
USE_LITE_KERNEL(expand_v2, kHost, kInt64, kAny, def);
USE_LITE_KERNEL(pow, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(beam_search_decode, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(where, kHost, kAny, kAny, def);
USE_LITE_KERNEL(multiclass_nms, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(multiclass_nms2, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(multiclass_nms3, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(roi_perspective_transform, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(deformable_conv, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(affine_grid, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(while, kHost, kAny, kAny, def);
USE_LITE_KERNEL(elementwise_add, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_add, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(elementwise_add, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(fusion_elementwise_add_activation, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_sub, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_sub, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(fusion_elementwise_sub_activation, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_mul, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(elementwise_mul, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_mul, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(fusion_elementwise_mul_activation, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(fusion_elementwise_mul_activation, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(elementwise_max, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(fusion_elementwise_max_activation, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_min, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(fusion_elementwise_min_activation, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_div, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_div, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(elementwise_div, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(fusion_elementwise_div_activation, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_mod, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(elementwise_pow, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(elementwise_pow, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(elementwise_floordiv, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(elementwise_floordiv, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(beam_search_decode, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(cast, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(range, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(range, kARM, kInt64, kNCHW, def);
USE_LITE_KERNEL(range, kARM, kInt32, kNCHW, def);
USE_LITE_KERNEL(relu, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(leaky_relu, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(relu_clipped, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(prelu, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(sigmoid, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(tanh, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(swish, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(relu6, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(log, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(exp, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(floor, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(hard_sigmoid, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(rsqrt, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(square, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(hard_swish, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(reciprocal, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(abs, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(thresholded_relu, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(elu, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(axpy, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(logical_xor, kHost, kAny, kAny, def);
USE_LITE_KERNEL(logical_and, kHost, kAny, kAny, def);
USE_LITE_KERNEL(logical_or, kHost, kAny, kAny, def);
USE_LITE_KERNEL(logical_not, kHost, kAny, kAny, def);
USE_LITE_KERNEL(gather, kHost, kFloat, kNCHW, int32int32);
USE_LITE_KERNEL(gather, kHost, kFloat, kNCHW, int64int64);
USE_LITE_KERNEL(gather, kHost, kFloat, kNCHW, int64int32);
USE_LITE_KERNEL(gather, kHost, kFloat, kNCHW, int32int64);
USE_LITE_KERNEL(lrn, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(print, kHost, kAny, kAny, def);
USE_LITE_KERNEL(tril_triu, kHost, kAny, kNCHW, float32);
USE_LITE_KERNEL(increment, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(box_clip, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(gather_nd, kHost, kAny, kAny, def);
USE_LITE_KERNEL(deformable_conv, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(fill_any_like, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(fill_zeros_like, kHost, kAny, kNCHW, def);
USE_LITE_KERNEL(sequence_expand, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(matmul_v2, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(write_to_array, kHost, kAny, kAny, def);
USE_LITE_KERNEL(arg_max, kHost, kAny, kNCHW, fp32);
USE_LITE_KERNEL(arg_max, kHost, kAny, kNCHW, int64);
USE_LITE_KERNEL(arg_max, kHost, kAny, kNCHW, int32);
USE_LITE_KERNEL(arg_max, kHost, kAny, kNCHW, int16);
USE_LITE_KERNEL(arg_max, kHost, kAny, kNCHW, uint8);
USE_LITE_KERNEL(polygon_box_transform, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(sequence_conv, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(top_k_v2, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(beam_search, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(read_from_array, kHost, kAny, kAny, def);
USE_LITE_KERNEL(cumsum, kHost, kFloat, kAny, float32);
USE_LITE_KERNEL(cumsum, kHost, kFloat, kAny, int32);
USE_LITE_KERNEL(cumsum, kHost, kFloat, kAny, int64);
USE_LITE_KERNEL(sequence_expand, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(box_coder, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(batch_norm, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(sync_batch_norm, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(clip, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(sequence_pool, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(sequence_pad, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(sequence_pad, kHost, kFloat, kNCHW, int32);
USE_LITE_KERNEL(sequence_pad, kHost, kFloat, kNCHW, int64);
USE_LITE_KERNEL(concat, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(shuffle_channel, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(crop, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(lookup_table_dequant, kARM, kAny, kNCHW, def);
USE_LITE_KERNEL(dropout, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(is_empty, kHost, kAny, kAny, def);
USE_LITE_KERNEL(sum, kARM, kFloat, kNCHW, sum_i32);
USE_LITE_KERNEL(sum, kARM, kFloat, kNCHW, sum_i64);
USE_LITE_KERNEL(sum, kARM, kFloat, kNCHW, sum_fp32);
USE_LITE_KERNEL(pad2d, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(sequence_softmax, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(unstack, kHost, kFloat, kAny, def);
USE_LITE_KERNEL(unstack, kHost, kFloat, kAny, unstack_int32);
USE_LITE_KERNEL(collect_fpn_proposals, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(grid_sampler, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(mul, kARM, kFloat, kNCHW, def);
USE_LITE_KERNEL(scatter_nd_add, kHost, kFloat, kNCHW, def);
USE_LITE_KERNEL(fill_zeros_like, kHost, kFloat, kNCHW, float32);
USE_LITE_KERNEL(fill_zeros_like, kHost, kFloat, kNCHW, int32);
USE_LITE_KERNEL(fill_zeros_like, kHost, kFloat, kNCHW, int64);
USE_LITE_KERNEL(reduce_min, kARM, kFloat, kNCHW, def);