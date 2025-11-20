use crate::types::Params;

pub fn unpack_params(params: Params) -> (u32, u32, u8) {
    (params.width, params.height, params.channels)
}