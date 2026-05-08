/*
This LSM6DSO driver is based on STMicroelectronics stm32-lsm6dso / LSM6DSO component driver.
Original copyright: STMicroelectronics.
Original license: BSD-3-Clause.

Modifications:
- Ported IO layer to W-Link / NeonRTOS I2C interface.
- Adapted configuration macros and operation status mapping.
- Removed STM32 BSP object/context dependency.
*/

#ifndef LSM6DSO_REGISTER_DEF_H
#define LSM6DSO_REGISTER_DEF_H

typedef union {
    int16_t i16bit[3];
    uint8_t u8bit[6];
} axis3bit16_t;

typedef union {
    int16_t i16bit;
    uint8_t u8bit[2];
} axis1bit16_t;

typedef union {
    int32_t i32bit[3];
    uint8_t u8bit[12];
} axis3bit32_t;

typedef union {
    int32_t i32bit;
    uint8_t u8bit[4];
} axis1bit32_t;

typedef struct bitwise_t{
    uint8_t bit0       : 1;
    uint8_t bit1       : 1;
    uint8_t bit2       : 1;
    uint8_t bit3       : 1;
    uint8_t bit4       : 1;
    uint8_t bit5       : 1;
    uint8_t bit6       : 1;
    uint8_t bit7       : 1;
} bitwise;

#define PROPERTY_DISABLE                (0U)
#define PROPERTY_ENABLE                 (1U)

#define LSM6DSO_DEV_ID                  0x6C

#define LSM6DSO_FUNC_CFG_ACCESS              0x01U
typedef struct LSM6DSO_Func_Cfg_Access_t {
  uint8_t not_used_01              : 6;
  uint8_t reg_access               : 2; /* shub_reg_access + func_cfg_access */
} LSM6DSO_Func_Cfg_Access;

#define LSM6DSO_PIN_CTRL                     0x02U
typedef struct LSM6DSO_Pin_Ctrl_t {
  uint8_t not_used_01              : 6;
  uint8_t sdo_pu_en                : 1;
  uint8_t ois_pu_dis               : 1;
} LSM6DSO_Pin_Ctrl;

#define LSM6DSO_FIFO_CTRL1                   0x07U
typedef struct LSM6DSO_FIFO_Ctrl1_t {
  uint8_t wtm                      : 8;
} LSM6DSO_FIFO_Ctrl1;

#define LSM6DSO_FIFO_CTRL2                   0x08U
typedef struct LSM6DSO_FIFO_Ctrl2_t {
  uint8_t wtm                      : 1;
  uint8_t uncoptr_rate             : 2;
  uint8_t not_used_01              : 1;
  uint8_t odrchg_en                : 1;
  uint8_t not_used_02              : 1;
  uint8_t fifo_compr_rt_en         : 1;
  uint8_t stop_on_wtm              : 1;
} LSM6DSO_FIFO_Ctrl2;

#define LSM6DSO_FIFO_CTRL3                   0x09U
typedef struct LSM6DSO_FIFO_Ctrl3_t {
  uint8_t bdr_xl                   : 4;
  uint8_t bdr_gy                   : 4;
} LSM6DSO_FIFO_Ctrl3;

#define LSM6DSO_FIFO_CTRL4                   0x0AU
typedef struct LSM6DSO_FIFO_Ctrl4_t {
  uint8_t fifo_mode                : 3;
  uint8_t not_used_01              : 1;
  uint8_t odr_t_batch              : 2;
  uint8_t odr_ts_batch             : 2;
} LSM6DSO_FIFO_Ctrl4;

#define LSM6DSO_COUNTER_BDR_REG1             0x0BU
typedef struct LSM6DSO_Counter_BDR_Reg1_t {
  uint8_t cnt_bdr_th               : 3;
  uint8_t not_used_01              : 2;
  uint8_t trig_counter_bdr         : 1;
  uint8_t rst_counter_bdr          : 1;
  uint8_t dataready_pulsed         : 1;
} LSM6DSO_Counter_BDR_Reg1;

#define LSM6DSO_COUNTER_BDR_REG2             0x0CU
typedef struct LSM6DSO_Counter_BDR_Reg2_t {
  uint8_t cnt_bdr_th               : 8;
} LSM6DSO_Counter_BDR_Reg2;

#define LSM6DSO_INT1_CTRL  0x0D
typedef struct LSM6DSO_Int1_Ctrl_t {
  uint8_t int1_drdy_xl             : 1;
  uint8_t int1_drdy_g              : 1;
  uint8_t int1_boot                : 1;
  uint8_t int1_fifo_th             : 1;
  uint8_t int1_fifo_ovr            : 1;
  uint8_t int1_fifo_full           : 1;
  uint8_t int1_cnt_bdr             : 1;
  uint8_t den_drdy_flag            : 1;
} LSM6DSO_Int1_Ctrl;

#define LSM6DSO_INT2_CTRL                    0x0EU
typedef struct LSM6DSO_Int2_Ctrl_t {
  uint8_t int2_drdy_xl             : 1;
  uint8_t int2_drdy_g              : 1;
  uint8_t int2_drdy_temp           : 1;
  uint8_t int2_fifo_th             : 1;
  uint8_t int2_fifo_ovr            : 1;
  uint8_t int2_fifo_full           : 1;
  uint8_t int2_cnt_bdr             : 1;
  uint8_t not_used_01              : 1;
} LSM6DSO_Int2_Ctrl;

#define LSM6DSO_WHO_AM_I                     0x0FU
#define LSM6DSO_CTRL1_XL                     0x10U
typedef struct LSM6DSO_Ctrl1_XL_t {
  uint8_t not_used_01              : 1;
  uint8_t lpf2_xl_en               : 1;
  uint8_t fs_xl                    : 2;
  uint8_t odr_xl                   : 4;
} LSM6DSO_Ctrl1_XL;

#define LSM6DSO_CTRL2_G                      0x11U
typedef struct LSM6DSO_Ctrl2_G_t {
  uint8_t not_used_01              : 1;
  uint8_t fs_g                     : 3; /* fs_125 + fs_g */
  uint8_t odr_g                    : 4;
} LSM6DSO_Ctrl2_G;

#define LSM6DSO_CTRL3_C                      0x12U
typedef struct LSM6DSO_Ctrl3_C_t {
  uint8_t sw_reset                 : 1;
  uint8_t not_used_01              : 1;
  uint8_t if_inc                   : 1;
  uint8_t sim                      : 1;
  uint8_t pp_od                    : 1;
  uint8_t h_lactive                : 1;
  uint8_t bdu                      : 1;
  uint8_t boot                     : 1;
} LSM6DSO_Ctrl3_C;

#define LSM6DSO_CTRL4_C                      0x13U
typedef struct LSM6DSO_Ctrl4_C_t {
  uint8_t not_used_01              : 1;
  uint8_t lpf1_sel_g               : 1;
  uint8_t i2c_disable              : 1;
  uint8_t drdy_mask                : 1;
  uint8_t not_used_02              : 1;
  uint8_t int2_on_int1             : 1;
  uint8_t sleep_g                  : 1;
  uint8_t not_used_03              : 1;
} LSM6DSO_Ctrl4_C;

#define LSM6DSO_CTRL5_C                      0x14U
typedef struct LSM6DSO_Ctrl5_C_t {
  uint8_t st_xl                    : 2;
  uint8_t st_g                     : 2;
  uint8_t not_used_01              : 1;
  uint8_t rounding                 : 2;
  uint8_t xl_ulp_en                : 1;
} LSM6DSO_Ctrl5_C;

#define LSM6DSO_CTRL6_C                      0x15U
typedef struct LSM6DSO_Ctrl6_C_t {
  uint8_t ftype                    : 3;
  uint8_t usr_off_w                : 1;
  uint8_t xl_hm_mode               : 1;
  uint8_t den_mode                 : 3;   /* trig_en + lvl1_en + lvl2_en */
} LSM6DSO_Ctrl6_C;

#define LSM6DSO_CTRL7_G                      0x16U
typedef struct LSM6DSO_Ctrl7_G_t {
  uint8_t ois_on                   : 1;
  uint8_t usr_off_on_out           : 1;
  uint8_t ois_on_en                : 1;
  uint8_t not_used_01              : 1;
  uint8_t hpm_g                    : 2;
  uint8_t hp_en_g                  : 1;
  uint8_t g_hm_mode                : 1;
} LSM6DSO_Ctrl7_G;

#define LSM6DSO_CTRL8_XL                     0x17U
typedef struct LSM6DSO_Ctrl8_XL_t {
  uint8_t low_pass_on_6d           : 1;
  uint8_t xl_fs_mode               : 1;
  uint8_t hp_slope_xl_en           : 1;
  uint8_t fastsettl_mode_xl        : 1;
  uint8_t hp_ref_mode_xl           : 1;
  uint8_t hpcf_xl                  : 3;
} LSM6DSO_Ctrl8_XL;

#define LSM6DSO_CTRL9_XL                     0x18U
typedef struct LSM6DSO_Ctrl9_XL_t {
  uint8_t not_used_01              : 1;
  uint8_t i3c_disable              : 1;
  uint8_t den_lh                   : 1;
  uint8_t den_xl_g                 : 2;   /* den_xl_en + den_xl_g */
  uint8_t den_z                    : 1;
  uint8_t den_y                    : 1;
  uint8_t den_x                    : 1;
} LSM6DSO_Ctrl9_XL;

#define LSM6DSO_CTRL10_C                     0x19U
typedef struct LSM6DSO_Ctrl10_C_t {
  uint8_t not_used_01              : 5;
  uint8_t timestamp_en             : 1;
  uint8_t not_used_02              : 2;
} LSM6DSO_Ctrl10_C;

#define LSM6DSO_ALL_INT_SRC                  0x1AU
typedef struct LSM6DSO_All_Int_Src_t {
  uint8_t ff_ia                    : 1;
  uint8_t wu_ia                    : 1;
  uint8_t single_tap               : 1;
  uint8_t double_tap               : 1;
  uint8_t d6d_ia                   : 1;
  uint8_t sleep_change_ia          : 1;
  uint8_t not_used_01              : 1;
  uint8_t timestamp_endcount       : 1;
} LSM6DSO_All_Int_Src;

#define LSM6DSO_WAKE_UP_SRC                  0x1BU
typedef struct LSM6DSO_Wake_Up_Src_t {
  uint8_t z_wu                     : 1;
  uint8_t y_wu                     : 1;
  uint8_t x_wu                     : 1;
  uint8_t wu_ia                    : 1;
  uint8_t sleep_state              : 1;
  uint8_t ff_ia                    : 1;
  uint8_t sleep_change_ia          : 1;
  uint8_t not_used_01              : 1;
} LSM6DSO_Wake_Up_Src;

#define LSM6DSO_TAP_SRC                      0x1CU
typedef struct LSM6DSO_Tap_Src_t {
  uint8_t z_tap                    : 1;
  uint8_t y_tap                    : 1;
  uint8_t x_tap                    : 1;
  uint8_t tap_sign                 : 1;
  uint8_t double_tap               : 1;
  uint8_t single_tap               : 1;
  uint8_t tap_ia                   : 1;
  uint8_t not_used_02              : 1;
} LSM6DSO_Tap_Src;

#define LSM6DSO_D6D_SRC                      0x1DU
typedef struct LSM6DSO_D6D_Src_t {
  uint8_t xl                       : 1;
  uint8_t xh                       : 1;
  uint8_t yl                       : 1;
  uint8_t yh                       : 1;
  uint8_t zl                       : 1;
  uint8_t zh                       : 1;
  uint8_t d6d_ia                   : 1;
  uint8_t den_drdy                 : 1;
} LSM6DSO_D6D_Src;

#define LSM6DSO_STATUS_REG                   0x1EU
typedef struct LSM6DSO_Status_Reg_t {
  uint8_t xlda                     : 1;
  uint8_t gda                      : 1;
  uint8_t tda                      : 1;
  uint8_t not_used_01              : 5;
} LSM6DSO_Status_Reg;

#define LSM6DSO_STATUS_SPIAUX                0x1EU
typedef struct LSM6DSO_Status_SpiAux_t {
  uint8_t xlda                     : 1;
  uint8_t gda                      : 1;
  uint8_t gyro_settling            : 1;
  uint8_t not_used_01              : 5;
} LSM6DSO_Status_SpiAux;

#define LSM6DSO_OUT_TEMP_L                   0x20U
#define LSM6DSO_OUT_TEMP_H                   0x21U
#define LSM6DSO_OUTX_L_G                     0x22U
#define LSM6DSO_OUTX_H_G                     0x23U
#define LSM6DSO_OUTY_L_G                     0x24U
#define LSM6DSO_OUTY_H_G                     0x25U
#define LSM6DSO_OUTZ_L_G                     0x26U
#define LSM6DSO_OUTZ_H_G                     0x27U
#define LSM6DSO_OUTX_L_A                     0x28U
#define LSM6DSO_OUTX_H_A                     0x29U
#define LSM6DSO_OUTY_L_A                     0x2AU
#define LSM6DSO_OUTY_H_A                     0x2BU
#define LSM6DSO_OUTZ_L_A                     0x2CU
#define LSM6DSO_OUTZ_H_A                     0x2DU
#define LSM6DSO_EMB_FUNC_STATUS_MAINPAGE     0x35U
typedef struct LSM6DSO_EMB_Func_Status_MainPage_t {
  uint8_t not_used_01             : 3;
  uint8_t is_step_det             : 1;
  uint8_t is_tilt                 : 1;
  uint8_t is_sigmot               : 1;
  uint8_t not_used_02             : 1;
  uint8_t is_fsm_lc               : 1;
} LSM6DSO_EMB_Func_Status_MainPage;

#define LSM6DSO_FSM_STATUS_A_MAINPAGE        0x36U
typedef struct LSM6DSO_FSM_Status_A_MainPage_t {
  uint8_t is_fsm1                 : 1;
  uint8_t is_fsm2                 : 1;
  uint8_t is_fsm3                 : 1;
  uint8_t is_fsm4                 : 1;
  uint8_t is_fsm5                 : 1;
  uint8_t is_fsm6                 : 1;
  uint8_t is_fsm7                 : 1;
  uint8_t is_fsm8                 : 1;
  } LSM6DSO_FSM_Status_A_MainPage;

#define LSM6DSO_FSM_STATUS_B_MAINPAGE        0x37U
typedef struct LSM6DSO_FSM_Status_B_MainPage_t {
  uint8_t is_fsm9                 : 1;
  uint8_t is_fsm10                : 1;
  uint8_t is_fsm11                : 1;
  uint8_t is_fsm12                : 1;
  uint8_t is_fsm13                : 1;
  uint8_t is_fsm14                : 1;
  uint8_t is_fsm15                : 1;
  uint8_t is_fsm16                : 1;
} LSM6DSO_FSM_Status_B_MainPage;

#define LSM6DSO_STATUS_MASTER_MAINPAGE       0x39U
typedef struct LSM6DSO_Status_Master_MainPage_t {
  uint8_t sens_hub_endop          : 1;
  uint8_t not_used_01             : 2;
  uint8_t slave0_nack             : 1;
  uint8_t slave1_nack             : 1;
  uint8_t slave2_nack             : 1;
  uint8_t slave3_nack             : 1;
  uint8_t wr_once_done            : 1;
} LSM6DSO_Status_Master_MainPage;

#define LSM6DSO_FIFO_STATUS1                 0x3AU
typedef struct LSM6DSO_FIFO_Status1_t {
  uint8_t diff_fifo                : 8;
} LSM6DSO_FIFO_Status1;

#define LSM6DSO_FIFO_STATUS2                 0x3B
typedef struct LSM6DSO_FIFO_Status2_t {
  uint8_t diff_fifo                : 2;
  uint8_t not_used_01              : 1;
  uint8_t over_run_latched         : 1;
  uint8_t counter_bdr_ia           : 1;
  uint8_t fifo_full_ia             : 1;
  uint8_t fifo_ovr_ia              : 1;
  uint8_t fifo_wtm_ia              : 1;
} LSM6DSO_FIFO_Status2;

#define LSM6DSO_TIMESTAMP0                   0x40U
#define LSM6DSO_TIMESTAMP1                   0x41U
#define LSM6DSO_TIMESTAMP2                   0x42U
#define LSM6DSO_TIMESTAMP3                   0x43U
#define LSM6DSO_TAP_CFG0                     0x56U
typedef struct LSM6DSO_Tap_Cfg0_t {
  uint8_t lir                      : 1;
  uint8_t tap_z_en                 : 1;
  uint8_t tap_y_en                 : 1;
  uint8_t tap_x_en                 : 1;
  uint8_t slope_fds                : 1;
  uint8_t sleep_status_on_int      : 1;
  uint8_t int_clr_on_read          : 1;
  uint8_t not_used_01              : 1;
} LSM6DSO_Tap_Cfg0;

#define LSM6DSO_TAP_CFG1                     0x57U
typedef struct LSM6DSO_Tap_Cfg1_t {
  uint8_t tap_ths_x                : 5;
  uint8_t tap_priority             : 3;
} LSM6DSO_Tap_Cfg1;

#define LSM6DSO_TAP_CFG2                     0x58U
typedef struct LSM6DSO_Tap_Cfg2_t {
  uint8_t tap_ths_y                : 5;
  uint8_t inact_en                 : 2;
  uint8_t interrupts_enable        : 1;
} LSM6DSO_Tap_Cfg2;

#define LSM6DSO_TAP_THS_6D                   0x59U
typedef struct LSM6DSO_Tap_Ths_6D_t {
  uint8_t tap_ths_z                : 5;
  uint8_t sixd_ths                 : 2;
  uint8_t d4d_en                   : 1;
} LSM6DSO_Tap_Ths_6D;

#define LSM6DSO_INT_DUR2                     0x5AU
typedef struct LSM6DSO_Int_Dur2_t {
  uint8_t shock                    : 2;
  uint8_t quiet                    : 2;
  uint8_t dur                      : 4;
} LSM6DSO_Int_Dur2;

#define LSM6DSO_WAKE_UP_THS                  0x5BU
typedef struct LSM6DSO_Wake_Up_Ths_t {
  uint8_t wk_ths                   : 6;
  uint8_t usr_off_on_wu            : 1;
  uint8_t single_double_tap        : 1;
} LSM6DSO_Wake_Up_Ths;

#define LSM6DSO_WAKE_UP_DUR                  0x5CU
typedef struct LSM6DSO_Wake_Up_Dur_t {
  uint8_t sleep_dur                : 4;
  uint8_t wake_ths_w               : 1;
  uint8_t wake_dur                 : 2;
  uint8_t ff_dur                   : 1;
} LSM6DSO_Wake_Up_Dur;

#define LSM6DSO_FREE_FALL                    0x5DU
typedef struct LSM6DSO_Free_Fall_t {
  uint8_t ff_ths                   : 3;
  uint8_t ff_dur                   : 5;
} LSM6DSO_Free_Fall;

#define LSM6DSO_MD1_CFG                      0x5EU
typedef struct LSM6DSO_MD1_Cfg_t {
  uint8_t int1_shub                : 1;
  uint8_t int1_emb_func            : 1;
  uint8_t int1_6d                  : 1;
  uint8_t int1_double_tap          : 1;
  uint8_t int1_ff                  : 1;
  uint8_t int1_wu                  : 1;
  uint8_t int1_single_tap          : 1;
  uint8_t int1_sleep_change        : 1;
} LSM6DSO_MD1_Cfg;

#define LSM6DSO_MD2_CFG                      0x5FU
typedef struct LSM6DSO_MD2_Cfg_t {
  uint8_t int2_timestamp           : 1;
  uint8_t int2_emb_func            : 1;
  uint8_t int2_6d                  : 1;
  uint8_t int2_double_tap          : 1;
  uint8_t int2_ff                  : 1;
  uint8_t int2_wu                  : 1;
  uint8_t int2_single_tap          : 1;
  uint8_t int2_sleep_change        : 1;
} LSM6DSO_MD2_Cfg;

#define LSM6DSO_I3C_BUS_AVB                  0x62U
typedef struct LSM6DSO_I3C_Bus_Avb_t {
  uint8_t pd_dis_int1              : 1;
  uint8_t not_used_01              : 2;
  uint8_t i3c_bus_avb_sel          : 2;
  uint8_t not_used_02              : 3;
} LSM6DSO_I3C_Bus_Avb;

#define LSM6DSO_INTERNAL_FREQ_FINE           0x63U
typedef struct LSM6DSO_Internal_Freq_Fine_t {
  uint8_t freq_fine                : 8;
} LSM6DSO_Internal_Freq_Fine;

#define LSM6DSO_INT_OIS                      0x6FU
typedef struct LSM6DSO_Int_Ois_t {
  uint8_t st_xl_ois                : 2;
  uint8_t not_used_01              : 3;
  uint8_t den_lh_ois               : 1;
  uint8_t lvl2_ois                 : 1;
  uint8_t int2_drdy_ois            : 1;
} LSM6DSO_Int_Ois;

#define LSM6DSO_CTRL1_OIS                    0x70U
typedef struct LSM6DSO_Ctrl1_Ois_t {
  uint8_t ois_en_spi2              : 1;
  uint8_t fs_g_ois                 : 3; /* fs_125_ois + fs[1:0]_g_ois */
  uint8_t mode4_en                 : 1;
  uint8_t sim_ois                  : 1;
  uint8_t lvl1_ois                  : 1;
  uint8_t not_used_01              : 1;
} LSM6DSO_Ctrl1_Ois;

#define LSM6DSO_CTRL2_OIS                    0x71U
typedef struct LSM6DSO_Ctrl2_Ois_t {
  uint8_t hp_en_ois                : 1;
  uint8_t ftype_ois                : 2;
  uint8_t not_used_01              : 1;
  uint8_t hpm_ois                  : 2;
  uint8_t not_used_02              : 2;
} LSM6DSO_Ctrl2_Ois;

#define LSM6DSO_CTRL3_OIS                    0x72U
typedef struct LSM6DSO_Ctrl3_Ois_t {
  uint8_t st_ois_clampdis          : 1;
  uint8_t st_ois                   : 2;
  uint8_t filter_xl_conf_ois       : 3;
  uint8_t fs_xl_ois                : 2;
} LSM6DSO_Ctrl3_Ois;

#define LSM6DSO_X_OFS_USR                    0x73U
#define LSM6DSO_Y_OFS_USR                    0x74U
#define LSM6DSO_Z_OFS_USR                    0x75U
#define LSM6DSO_FIFO_DATA_OUT_TAG            0x78U
typedef struct LSM6DSO_FIFO_Data_Out_Tag_t {
  uint8_t tag_parity               : 1;
  uint8_t tag_cnt                  : 2;
  uint8_t tag_sensor               : 5;
} LSM6DSO_FIFO_Data_Out_Tag;

#define LSM6DSO_FIFO_DATA_OUT_X_L            0x79U
#define LSM6DSO_FIFO_DATA_OUT_X_H            0x7AU
#define LSM6DSO_FIFO_DATA_OUT_Y_L            0x7BU
#define LSM6DSO_FIFO_DATA_OUT_Y_H            0x7CU
#define LSM6DSO_FIFO_DATA_OUT_Z_L            0x7DU
#define LSM6DSO_FIFO_DATA_OUT_Z_H            0x7EU
#define LSM6DSO_PAGE_SEL                     0x02U
typedef struct LSM6DSO_Page_Sel_t {
  uint8_t not_used_01              : 4;
  uint8_t page_sel                 : 4;
} LSM6DSO_Page_Sel;

#define LSM6DSO_EMB_FUNC_EN_A                0x04U
typedef struct LSM6DSO_EMB_Func_En_A_t {
  uint8_t not_used_01              : 3;
  uint8_t pedo_en                  : 1;
  uint8_t tilt_en                  : 1;
  uint8_t sign_motion_en           : 1;
  uint8_t not_used_02              : 2;
} LSM6DSO_EMB_Func_En_A;

#define LSM6DSO_EMB_FUNC_EN_B                0x05U
typedef struct LSM6DSO_EMB_Func_En_B_t {
  uint8_t fsm_en                   : 1;
  uint8_t not_used_01              : 2;
  uint8_t fifo_compr_en            : 1;
  uint8_t pedo_adv_en              : 1;
  uint8_t not_used_02              : 3;
} LSM6DSO_EMB_Func_En_B;

#define LSM6DSO_PAGE_ADDRESS                 0x08U
typedef struct LSM6DSO_Page_Address_t {
  uint8_t page_addr                : 8;
} LSM6DSO_Page_Address;

#define LSM6DSO_PAGE_VALUE                   0x09U
typedef struct LSM6DSO_Page_Value_t {
  uint8_t page_value               : 8;
} LSM6DSO_Page_Value;

#define LSM6DSO_EMB_FUNC_INT1                0x0AU
typedef struct LSM6DSO_EMB_Func_Int1_t {
  uint8_t not_used_01              : 3;
  uint8_t int1_step_detector       : 1;
  uint8_t int1_tilt                : 1;
  uint8_t int1_sig_mot             : 1;
  uint8_t not_used_02              : 1;
  uint8_t int1_fsm_lc              : 1;
} LSM6DSO_EMB_Func_Int1;

#define LSM6DSO_FSM_INT1_A                   0x0BU
typedef struct LSM6DSO_FSM_Int1_A_t {
  uint8_t int1_fsm1                : 1;
  uint8_t int1_fsm2                : 1;
  uint8_t int1_fsm3                : 1;
  uint8_t int1_fsm4                : 1;
  uint8_t int1_fsm5                : 1;
  uint8_t int1_fsm6                : 1;
  uint8_t int1_fsm7                : 1;
  uint8_t int1_fsm8                : 1;
} LSM6DSO_FSM_Int1_A;

#define LSM6DSO_FSM_INT1_B                   0x0CU
typedef struct LSM6DSO_FSM_Int1_B_t {
  uint8_t int1_fsm9                : 1;
  uint8_t int1_fsm10               : 1;
  uint8_t int1_fsm11               : 1;
  uint8_t int1_fsm12               : 1;
  uint8_t int1_fsm13               : 1;
  uint8_t int1_fsm14               : 1;
  uint8_t int1_fsm15               : 1;
  uint8_t int1_fsm16               : 1;
} LSM6DSO_FSM_Int1_B;

#define LSM6DSO_EMB_FUNC_INT2                0x0EU
typedef struct LSM6DSO_EMB_Func_Int2_t {
  uint8_t not_used_01              : 3;
  uint8_t int2_step_detector       : 1;
  uint8_t int2_tilt                : 1;
  uint8_t int2_sig_mot             : 1;
  uint8_t not_used_02              : 1;
  uint8_t int2_fsm_lc              : 1;
} LSM6DSO_EMB_Func_Int2;

#define LSM6DSO_FSM_INT2_A                   0x0FU
typedef struct LSM6DSO_FSM_Int2_A_t {
  uint8_t int2_fsm1                : 1;
  uint8_t int2_fsm2                : 1;
  uint8_t int2_fsm3                : 1;
  uint8_t int2_fsm4                : 1;
  uint8_t int2_fsm5                : 1;
  uint8_t int2_fsm6                : 1;
  uint8_t int2_fsm7                : 1;
  uint8_t int2_fsm8                : 1;
} LSM6DSO_FSM_Int2_A;

#define LSM6DSO_FSM_INT2_B                   0x10U
typedef struct LSM6DSO_FSM_Int2_B_t {
  uint8_t int2_fsm9                : 1;
  uint8_t int2_fsm10               : 1;
  uint8_t int2_fsm11               : 1;
  uint8_t int2_fsm12               : 1;
  uint8_t int2_fsm13               : 1;
  uint8_t int2_fsm14               : 1;
  uint8_t int2_fsm15               : 1;
  uint8_t int2_fsm16               : 1;
} LSM6DSO_FSM_Int2_B;

#define LSM6DSO_EMB_FUNC_STATUS              0x12U
typedef struct LSM6DSO_EMB_Func_Status_t {
  uint8_t not_used_01              : 3;
  uint8_t is_step_det              : 1;
  uint8_t is_tilt                  : 1;
  uint8_t is_sigmot                : 1;
  uint8_t not_used_02              : 1;
  uint8_t is_fsm_lc                : 1;
} LSM6DSO_EMB_Func_Status;

#define LSM6DSO_FSM_STATUS_A                 0x13U
typedef struct LSM6DSO_FSM_Status_A_t {
  uint8_t is_fsm1                  : 1;
  uint8_t is_fsm2                  : 1;
  uint8_t is_fsm3                  : 1;
  uint8_t is_fsm4                  : 1;
  uint8_t is_fsm5                  : 1;
  uint8_t is_fsm6                  : 1;
  uint8_t is_fsm7                  : 1;
  uint8_t is_fsm8                  : 1;
} LSM6DSO_FSM_Status_A;

#define LSM6DSO_FSM_STATUS_B                 0x14U
typedef struct LSM6DSO_FSM_Status_B_t {
  uint8_t is_fsm9                  : 1;
  uint8_t is_fsm10                 : 1;
  uint8_t is_fsm11                 : 1;
  uint8_t is_fsm12                 : 1;
  uint8_t is_fsm13                 : 1;
  uint8_t is_fsm14                 : 1;
  uint8_t is_fsm15                 : 1;
  uint8_t is_fsm16                 : 1;
} LSM6DSO_FSM_Status_B;

#define LSM6DSO_PAGE_RW                      0x17U
typedef struct LSM6DSO_Page_RW_t {
  uint8_t not_used_01              : 5;
  uint8_t page_rw                  : 2;  /* page_write + page_read */
  uint8_t emb_func_lir             : 1;
} LSM6DSO_Page_RW;

#define LSM6DSO_EMB_FUNC_FIFO_CFG             0x44U
typedef struct LSM6DSO_EMB_Func_FIFO_Cfg_t {
  uint8_t not_used_00              : 6;
  uint8_t pedo_fifo_en             : 1;
  uint8_t not_used_01              : 1;
} LSM6DSO_EMB_Func_FIFO_Cfg;

#define LSM6DSO_FSM_ENABLE_A                 0x46U
typedef struct LSM6DSO_FSM_Enable_A_t {
  uint8_t fsm1_en                  : 1;
  uint8_t fsm2_en                  : 1;
  uint8_t fsm3_en                  : 1;
  uint8_t fsm4_en                  : 1;
  uint8_t fsm5_en                  : 1;
  uint8_t fsm6_en                  : 1;
  uint8_t fsm7_en                  : 1;
  uint8_t fsm8_en                  : 1;
} LSM6DSO_FSM_Enable_A;

#define LSM6DSO_FSM_ENABLE_B                 0x47U
typedef struct LSM6DSO_FSM_Enable_B_t {
  uint8_t fsm9_en                  : 1;
  uint8_t fsm10_en                 : 1;
  uint8_t fsm11_en                 : 1;
  uint8_t fsm12_en                 : 1;
  uint8_t fsm13_en                 : 1;
  uint8_t fsm14_en                 : 1;
  uint8_t fsm15_en                 : 1;
  uint8_t fsm16_en                 : 1;
} LSM6DSO_FSM_Enable_B;

#define LSM6DSO_FSM_LONG_COUNTER_L           0x48U
#define LSM6DSO_FSM_LONG_COUNTER_H           0x49U
#define LSM6DSO_FSM_LONG_COUNTER_CLEAR       0x4AU
typedef struct LSM6DSO_FSM_Long_Counter_Clear_t{
  uint8_t fsm_lc_clr               : 2;  /* fsm_lc_cleared + fsm_lc_clear */
  uint8_t not_used_01              : 6;
} LSM6DSO_FSM_Long_Counter_Clear;

#define LSM6DSO_FSM_OUTS1                    0x4CU
typedef struct LSM6DSO_FSM_Outs1_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs1;

#define LSM6DSO_FSM_OUTS2                    0x4DU
typedef struct LSM6DSO_FSM_Outs2_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs2;

#define LSM6DSO_FSM_OUTS3                    0x4EU
typedef struct LSM6DSO_FSM_Outs3_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs3;

#define LSM6DSO_FSM_OUTS4                    0x4FU
typedef struct LSM6DSO_FSM_Outs4_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs4;

#define LSM6DSO_FSM_OUTS5                    0x50U
typedef struct LSM6DSO_FSM_Outs5_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs5;

#define LSM6DSO_FSM_OUTS6                    0x51U
typedef struct LSM6DSO_FSM_Outs6_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs6;

#define LSM6DSO_FSM_OUTS7                    0x52U
typedef struct LSM6DSO_FSM_Outs7_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs7;

#define LSM6DSO_FSM_OUTS8                    0x53U
typedef struct LSM6DSO_FSM_Outs8_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs8;

#define LSM6DSO_FSM_OUTS9                    0x54U
typedef struct LSM6DSO_FSM_Outs9_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs9;

#define LSM6DSO_FSM_OUTS10                   0x55U
typedef struct LSM6DSO_FSM_Outs10_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs10;

#define LSM6DSO_FSM_OUTS11                   0x56U
typedef struct LSM6DSO_FSM_Outs11_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs11;

#define LSM6DSO_FSM_OUTS12                   0x57U
typedef struct LSM6DSO_FSM_Outs12_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs12;

#define LSM6DSO_FSM_OUTS13                   0x58U
typedef struct LSM6DSO_FSM_Outs13_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs13;

#define LSM6DSO_FSM_OUTS14                   0x59U
typedef struct LSM6DSO_FSM_Outs14_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs14;

#define LSM6DSO_FSM_OUTS15                   0x5AU
typedef struct LSM6DSO_FSM_Outs15_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs15;

#define LSM6DSO_FSM_OUTS16                   0x5BU
typedef struct LSM6DSO_FSM_Outs16_t {
  uint8_t n_v                      : 1;
  uint8_t p_v                      : 1;
  uint8_t n_z                      : 1;
  uint8_t p_z                      : 1;
  uint8_t n_y                      : 1;
  uint8_t p_y                      : 1;
  uint8_t n_x                      : 1;
  uint8_t p_x                      : 1;
} LSM6DSO_FSM_Outs16;

#define LSM6DSO_EMB_FUNC_ODR_CFG_B           0x5FU
typedef struct LSM6DSO_EMB_Func_ODR_Cfg_B_t{
  uint8_t not_used_01              : 3;
  uint8_t fsm_odr                  : 2;
  uint8_t not_used_02              : 3;
} LSM6DSO_EMB_Func_ODR_Cfg_B;

#define LSM6DSO_STEP_COUNTER_L               0x62U
#define LSM6DSO_STEP_COUNTER_H               0x63U
#define LSM6DSO_EMB_FUNC_SRC                 0x64U
typedef struct LSM6DSO_EMB_Func_Src_t {
  uint8_t not_used_01              : 2;
  uint8_t stepcounter_bit_set      : 1;
  uint8_t step_overflow            : 1;
  uint8_t step_count_delta_ia      : 1;
  uint8_t step_detected            : 1;
  uint8_t not_used_02              : 1;
  uint8_t pedo_rst_step            : 1;
} LSM6DSO_EMB_Func_Src;

#define LSM6DSO_EMB_FUNC_INIT_A              0x66U
typedef struct LSM6DSO_EMB_Func_Init_A_t {
  uint8_t not_used_01               : 3;
  uint8_t step_det_init             : 1;
  uint8_t tilt_init                 : 1;
  uint8_t sig_mot_init              : 1;
  uint8_t not_used_02               : 2;
} LSM6DSO_EMB_Func_Init_A;

#define LSM6DSO_EMB_FUNC_INIT_B              0x67U
typedef struct LSM6DSO_EMB_Func_Init_B_t {
  uint8_t fsm_init                 : 1;
  uint8_t not_used_01              : 2;
  uint8_t fifo_compr_init          : 1;
  uint8_t not_used_02              : 4;
} LSM6DSO_EMB_Func_Init_B;

#define LSM6DSO_MAG_SENSITIVITY_L            0xBAU
#define LSM6DSO_MAG_SENSITIVITY_H            0xBBU
#define LSM6DSO_MAG_OFFX_L                   0xC0U
#define LSM6DSO_MAG_OFFX_H                   0xC1U
#define LSM6DSO_MAG_OFFY_L                   0xC2U
#define LSM6DSO_MAG_OFFY_H                   0xC3U
#define LSM6DSO_MAG_OFFZ_L                   0xC4U
#define LSM6DSO_MAG_OFFZ_H                   0xC5U
#define LSM6DSO_MAG_SI_XX_L                  0xC6U
#define LSM6DSO_MAG_SI_XX_H                  0xC7U
#define LSM6DSO_MAG_SI_XY_L                  0xC8U
#define LSM6DSO_MAG_SI_XY_H                  0xC9U
#define LSM6DSO_MAG_SI_XZ_L                  0xCAU
#define LSM6DSO_MAG_SI_XZ_H                  0xCBU
#define LSM6DSO_MAG_SI_YY_L                  0xCCU
#define LSM6DSO_MAG_SI_YY_H                  0xCDU
#define LSM6DSO_MAG_SI_YZ_L                  0xCEU
#define LSM6DSO_MAG_SI_YZ_H                  0xCFU
#define LSM6DSO_MAG_SI_ZZ_L                  0xD0U
#define LSM6DSO_MAG_SI_ZZ_H                  0xD1U
#define LSM6DSO_MAG_CFG_A                    0xD4U
typedef struct LSM6DSO_Mag_Cfg_A_t {
  uint8_t mag_z_axis               : 3;
  uint8_t not_used_01              : 1;
  uint8_t mag_y_axis               : 3;
  uint8_t not_used_02              : 1;
} LSM6DSO_Mag_Cfg_A;

#define LSM6DSO_MAG_CFG_B                    0xD5U
typedef struct LSM6DSO_Mag_Cfg_B_t {
  uint8_t mag_x_axis               : 3;
  uint8_t not_used_01              : 5;
} LSM6DSO_Mag_Cfg_B;

#define LSM6DSO_FSM_LC_TIMEOUT_L             0x17AU
#define LSM6DSO_FSM_LC_TIMEOUT_H             0x17BU
#define LSM6DSO_FSM_PROGRAMS                 0x17CU
#define LSM6DSO_FSM_START_ADD_L              0x17EU
#define LSM6DSO_FSM_START_ADD_H              0x17FU
#define LSM6DSO_PEDO_CMD_REG                 0x183U
typedef struct LSM6DSO_Pedo_Cmd_Reg_t {
  uint8_t ad_det_en                : 1;
  uint8_t not_used_01              : 1;
  uint8_t fp_rejection_en          : 1;
  uint8_t carry_count_en           : 1;
  uint8_t not_used_02              : 4;
} LSM6DSO_Pedo_Cmd_Reg;

#define LSM6DSO_PEDO_DEB_STEPS_CONF          0x184U
#define LSM6DSO_PEDO_SC_DELTAT_L             0x1D0U
#define LSM6DSO_PEDO_SC_DELTAT_H             0x1D1U
#define LSM6DSO_SENSOR_HUB_1                 0x02U
typedef struct LSM6DSO_Sensor_Hub_1_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_1;

#define LSM6DSO_SENSOR_HUB_2                 0x03U
typedef struct LSM6DSO_Sensor_Hub_2_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_2;

#define LSM6DSO_SENSOR_HUB_3                 0x04U
typedef struct LSM6DSO_Sensor_Hub_3_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_3;

#define LSM6DSO_SENSOR_HUB_4                 0x05U
typedef struct LSM6DSO_Sensor_Hub_4_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_4;

#define LSM6DSO_SENSOR_HUB_5                 0x06U
typedef struct LSM6DSO_Sensor_Hub_5_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_5;

#define LSM6DSO_SENSOR_HUB_6                 0x07U
typedef struct LSM6DSO_Sensor_Hub_6_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_6;

#define LSM6DSO_SENSOR_HUB_7                 0x08U
typedef struct LSM6DSO_Sensor_Hub_7_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_7;

#define LSM6DSO_SENSOR_HUB_8                 0x09U
typedef struct LSM6DSO_Sensor_Hub_8_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_8;

#define LSM6DSO_SENSOR_HUB_9                 0x0AU
typedef struct LSM6DSO_Sensor_Hub_9_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_9;

#define LSM6DSO_SENSOR_HUB_10                0x0BU
typedef struct LSM6DSO_Sensor_Hub_10_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_10;

#define LSM6DSO_SENSOR_HUB_11                0x0CU
typedef struct LSM6DSO_Sensor_Hub_11_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_11;

#define LSM6DSO_SENSOR_HUB_12                0x0DU
typedef struct LSM6DSO_Sensor_Hub_12_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_12;

#define LSM6DSO_SENSOR_HUB_13                0x0EU
typedef struct LSM6DSO_Sensor_Hub_13_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_13;

#define LSM6DSO_SENSOR_HUB_14                0x0FU
typedef struct LSM6DSO_Sensor_Hub_14_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_14;

#define LSM6DSO_SENSOR_HUB_15                0x10U
typedef struct LSM6DSO_Sensor_Hub_15_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_15;

#define LSM6DSO_SENSOR_HUB_16                0x11U
typedef struct LSM6DSO_Sensor_Hub_16_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_16;

#define LSM6DSO_SENSOR_HUB_17                0x12U
typedef struct LSM6DSO_Sensor_Hub_17_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_17;

#define LSM6DSO_SENSOR_HUB_18                0x13U
typedef struct LSM6DSO_Sensor_Hub_18_t {
   uint8_t bit0                    : 1;
   uint8_t bit1                    : 1;
   uint8_t bit2                    : 1;
   uint8_t bit3                    : 1;
   uint8_t bit4                    : 1;
   uint8_t bit5                    : 1;
   uint8_t bit6                    : 1;
   uint8_t bit7                    : 1;
} LSM6DSO_Sensor_Hub_18;

#define LSM6DSO_MASTER_CONFIG                0x14U
typedef struct LSM6DSO_Master_Config_t {
  uint8_t aux_sens_on              : 2;
  uint8_t master_on                : 1;
  uint8_t shub_pu_en               : 1;
  uint8_t pass_through_mode        : 1;
  uint8_t start_config             : 1;
  uint8_t write_once               : 1;
  uint8_t rst_master_regs          : 1;
} LSM6DSO_Master_Config;

#define LSM6DSO_SLV0_ADD                     0x15U
typedef struct LSM6DSO_Slv0_Add_t {
  uint8_t rw_0                     : 1;
  uint8_t slave0                   : 7;
} LSM6DSO_Slv0_Add;

#define LSM6DSO_SLV0_SUBADD                  0x16U
typedef struct LSM6DSO_Slv0_SubAdd_t {
  uint8_t slave0_reg               : 8;
} LSM6DSO_Slv0_SubAdd;

#define LSM6DSO_SLV0_CONFIG                  0x17U
typedef struct LSM6DSO_Slv0_Config_t {
  uint8_t slave0_numop             : 3;
  uint8_t batch_ext_sens_0_en      : 1;
  uint8_t not_used_01              : 2;
  uint8_t shub_odr                 : 2;
} LSM6DSO_Slv0_Config;

#define LSM6DSO_SLV1_ADD                     0x18U
typedef struct LSM6DSO_Slv1_Add_t {
  uint8_t r_1                      : 1;
  uint8_t slave1_add               : 7;
} LSM6DSO_Slv1_Add;

#define LSM6DSO_SLV1_SUBADD                  0x19U
typedef struct LSM6DSO_Slv1_SubAdd_t {
  uint8_t slave1_reg               : 8;
} LSM6DSO_Slv1_SubAdd;

#define LSM6DSO_SLV1_CONFIG                  0x1AU
typedef struct LSM6DSO_Slv1_Config_t {
  uint8_t slave1_numop             : 3;
  uint8_t batch_ext_sens_1_en      : 1;
  uint8_t not_used_01              : 4;
} LSM6DSO_Slv1_Config;

#define LSM6DSO_SLV2_ADD                     0x1BU
typedef struct LSM6DSO_Slv2_Add_t {
  uint8_t r_2                      : 1;
  uint8_t slave2_add               : 7;
} LSM6DSO_Slv2_Add;

#define LSM6DSO_SLV2_SUBADD                  0x1CU
typedef struct LSM6DSO_Slv2_SubAdd_t {
  uint8_t slave2_reg               : 8;
} LSM6DSO_Slv2_SubAdd;

#define LSM6DSO_SLV2_CONFIG                  0x1DU
typedef struct LSM6DSO_Slv2_Config_t {
  uint8_t slave2_numop             : 3;
  uint8_t batch_ext_sens_2_en      : 1;
  uint8_t not_used_01              : 4;
} LSM6DSO_Slv2_Config;

#define LSM6DSO_SLV3_ADD                     0x1EU
typedef struct LSM6DSO_Slv3_Add_t {
  uint8_t r_3                      : 1;
  uint8_t slave3_add               : 7;
} LSM6DSO_Slv3_Add;

#define LSM6DSO_SLV3_SUBADD                  0x1FU
typedef struct LSM6DSO_Slv3_SubAdd_t {
  uint8_t slave3_reg               : 8;
} LSM6DSO_Slv3_SubAdd;

#define LSM6DSO_SLV3_CONFIG                  0x20U
typedef struct LSM6DSO_Slv3_Config_t {
  uint8_t slave3_numop             : 3;
  uint8_t  batch_ext_sens_3_en     : 1;
  uint8_t not_used_01              : 4;
} LSM6DSO_Slv3_Config;

#define LSM6DSO_DATAWRITE_SLV0               0x21U
typedef struct LSM6DSO_DataWrite_Src_Mode_Sub_Slv0_t {
  uint8_t slave0_dataw             : 8;
} LSM6DSO_DataWrite_Src_Mode_Sub_Slv0;

#define LSM6DSO_STATUS_MASTER                0x22U
typedef struct LSM6DSO_Status_Master_t {
  uint8_t sens_hub_endop           : 1;
  uint8_t not_used_01              : 2;
  uint8_t slave0_nack              : 1;
  uint8_t slave1_nack              : 1;
  uint8_t slave2_nack              : 1;
  uint8_t slave3_nack              : 1;
  uint8_t wr_once_done             : 1;
} LSM6DSO_Status_Master;

#define LSM6DSO_START_FSM_ADD                0x0400U

typedef union LSM6DSO_reg_t{
  LSM6DSO_Func_Cfg_Access               func_cfg_access;
  LSM6DSO_Pin_Ctrl                      pin_ctrl;
  LSM6DSO_FIFO_Ctrl1                    fifo_ctrl1;
  LSM6DSO_FIFO_Ctrl2                    fifo_ctrl2;
  LSM6DSO_FIFO_Ctrl3                    fifo_ctrl3;
  LSM6DSO_FIFO_Ctrl4                    fifo_ctrl4;
  LSM6DSO_Counter_BDR_Reg1              counter_bdr_reg1;
  LSM6DSO_Counter_BDR_Reg2              counter_bdr_reg2;
  LSM6DSO_Int1_Ctrl                     int1_ctrl;
  LSM6DSO_Int2_Ctrl                     int2_ctrl;
  LSM6DSO_Ctrl1_XL                      ctrl1_xl;
  LSM6DSO_Ctrl2_G                       ctrl2_g;
  LSM6DSO_Ctrl3_C                       ctrl3_c;
  LSM6DSO_Ctrl4_C                       ctrl4_c;
  LSM6DSO_Ctrl5_C                       ctrl5_c;
  LSM6DSO_Ctrl6_C                       ctrl6_c;
  LSM6DSO_Ctrl7_G                       ctrl7_g;
  LSM6DSO_Ctrl8_XL                      ctrl8_xl;
  LSM6DSO_Ctrl9_XL                      ctrl9_xl;
  LSM6DSO_Ctrl10_C                      ctrl10_c;
  LSM6DSO_All_Int_Src                   all_int_src;
  LSM6DSO_Wake_Up_Src                   wake_up_src;
  LSM6DSO_Tap_Src                       tap_src;
  LSM6DSO_D6D_Src                       d6d_src;
  LSM6DSO_Status_Reg                    status_reg;
  LSM6DSO_Status_SpiAux                 status_spiaux;
  LSM6DSO_FIFO_Status1                  fifo_status1;
  LSM6DSO_FIFO_Status2                  fifo_status2;
  LSM6DSO_Tap_Cfg0                      tap_cfg0;
  LSM6DSO_Tap_Cfg1                      tap_cfg1;
  LSM6DSO_Tap_Cfg2                      tap_cfg2;
  LSM6DSO_Tap_Ths_6D                    tap_ths_6d;
  LSM6DSO_Int_Dur2                      int_dur2;
  LSM6DSO_Wake_Up_Ths                   wake_up_ths;
  LSM6DSO_Wake_Up_Dur                   wake_up_dur;
  LSM6DSO_Free_Fall                     free_fall;
  LSM6DSO_MD1_Cfg                       md1_cfg;
  LSM6DSO_MD2_Cfg                       md2_cfg;
  LSM6DSO_I3C_Bus_Avb                   i3c_bus_avb;
  LSM6DSO_Internal_Freq_Fine            internal_freq_fine;
  LSM6DSO_Int_Ois                       int_ois;
  LSM6DSO_Ctrl1_Ois                     ctrl1_ois;
  LSM6DSO_Ctrl2_Ois                     ctrl2_ois;
  LSM6DSO_Ctrl3_Ois                     ctrl3_ois;
  LSM6DSO_FIFO_Data_Out_Tag             fifo_data_out_tag;
  LSM6DSO_Page_Sel                      page_sel;
  LSM6DSO_EMB_Func_En_A                 emb_func_en_a;
  LSM6DSO_EMB_Func_En_B                 emb_func_en_b;
  LSM6DSO_Page_Address                  page_address;
  LSM6DSO_Page_Value                    page_value;
  LSM6DSO_EMB_Func_Int1                 emb_func_int1;
  LSM6DSO_FSM_Int1_A                    fsm_int1_a;
  LSM6DSO_FSM_Int1_B                    fsm_int1_b;
  LSM6DSO_EMB_Func_Int2                 emb_func_int2;
  LSM6DSO_FSM_Int2_A                    fsm_int2_a;
  LSM6DSO_FSM_Int2_B                    fsm_int2_b;
  LSM6DSO_EMB_Func_Status               emb_func_status;
  LSM6DSO_FSM_Status_A                  fsm_status_a;
  LSM6DSO_FSM_Status_B                  fsm_status_b;
  LSM6DSO_Page_RW                       page_rw;
  LSM6DSO_EMB_Func_FIFO_Cfg             emb_func_fifo_cfg;
  LSM6DSO_FSM_Enable_A                  fsm_enable_a;
  LSM6DSO_FSM_Enable_B                  fsm_enable_b;
  LSM6DSO_FSM_Long_Counter_Clear        fsm_long_counter_clear;
  LSM6DSO_FSM_Outs1                     fsm_outs1;
  LSM6DSO_FSM_Outs2                     fsm_outs2;
  LSM6DSO_FSM_Outs3                     fsm_outs3;
  LSM6DSO_FSM_Outs4                     fsm_outs4;
  LSM6DSO_FSM_Outs5                     fsm_outs5;
  LSM6DSO_FSM_Outs6                     fsm_outs6;
  LSM6DSO_FSM_Outs7                     fsm_outs7;
  LSM6DSO_FSM_Outs8                     fsm_outs8;
  LSM6DSO_FSM_Outs9                     fsm_outs9;
  LSM6DSO_FSM_Outs10                    fsm_outs10;
  LSM6DSO_FSM_Outs11                    fsm_outs11;
  LSM6DSO_FSM_Outs12                    fsm_outs12;
  LSM6DSO_FSM_Outs13                    fsm_outs13;
  LSM6DSO_FSM_Outs14                    fsm_outs14;
  LSM6DSO_FSM_Outs15                    fsm_outs15;
  LSM6DSO_FSM_Outs16                    fsm_outs16;
  LSM6DSO_EMB_Func_ODR_Cfg_B            emb_func_odr_cfg_b;
  LSM6DSO_EMB_Func_Src                  emb_func_src;
  LSM6DSO_EMB_Func_Init_A               emb_func_init_a;
  LSM6DSO_EMB_Func_Init_B               emb_func_init_b;
  LSM6DSO_Mag_Cfg_A                     mag_cfg_a;
  LSM6DSO_Mag_Cfg_B                     mag_cfg_b;
  LSM6DSO_Pedo_Cmd_Reg                  pedo_cmd_reg;
  LSM6DSO_Sensor_Hub_1                  sensor_hub_1;
  LSM6DSO_Sensor_Hub_2                  sensor_hub_2;
  LSM6DSO_Sensor_Hub_3                  sensor_hub_3;
  LSM6DSO_Sensor_Hub_4                  sensor_hub_4;
  LSM6DSO_Sensor_Hub_5                  sensor_hub_5;
  LSM6DSO_Sensor_Hub_6                  sensor_hub_6;
  LSM6DSO_Sensor_Hub_7                  sensor_hub_7;
  LSM6DSO_Sensor_Hub_8                  sensor_hub_8;
  LSM6DSO_Sensor_Hub_9                  sensor_hub_9;
  LSM6DSO_Sensor_Hub_10                 sensor_hub_10;
  LSM6DSO_Sensor_Hub_11                 sensor_hub_11;
  LSM6DSO_Sensor_Hub_12                 sensor_hub_12;
  LSM6DSO_Sensor_Hub_13                 sensor_hub_13;
  LSM6DSO_Sensor_Hub_14                 sensor_hub_14;
  LSM6DSO_Sensor_Hub_15                 sensor_hub_15;
  LSM6DSO_Sensor_Hub_16                 sensor_hub_16;
  LSM6DSO_Sensor_Hub_17                 sensor_hub_17;
  LSM6DSO_Sensor_Hub_18                 sensor_hub_18;
  LSM6DSO_Master_Config                 master_config;
  LSM6DSO_Slv0_Add                      slv0_add;
  LSM6DSO_Slv0_SubAdd                   slv0_subadd;
  LSM6DSO_Slv0_Config                   slv0_config;
  LSM6DSO_Slv1_Add                      slv1_add;
  LSM6DSO_Slv1_SubAdd                   slv1_subadd;
  LSM6DSO_Slv1_Config                   slv1_config;
  LSM6DSO_Slv2_Add                      slv2_add;
  LSM6DSO_Slv2_SubAdd                   slv2_subadd;
  LSM6DSO_Slv2_Config                   slv2_config;
  LSM6DSO_Slv3_Add                      slv3_add;
  LSM6DSO_Slv3_SubAdd                   slv3_subadd;
  LSM6DSO_Slv3_Config                   slv3_config;
  LSM6DSO_DataWrite_Src_Mode_Sub_Slv0   datawrite_src_Mode_sub_slv0;
  LSM6DSO_Status_Master                 status_master;
  bitwise                               bitwise;
  uint8_t                               byte;
} LSM6DSO_Reg;

typedef enum LSM6DSO_FS_XL_t {
  LSM6DSO_2g   = 0,
  LSM6DSO_16g  = 1, /* if XL_FS_MODE = ‘1’ -> LSM6DSO_2g */
  LSM6DSO_4g   = 2,
  LSM6DSO_8g   = 3,
} LSM6DSO_FS_XL;

typedef enum LSM6DSO_ODR_XL_t {
  LSM6DSO_XL_ODR_OFF    = 0,
  LSM6DSO_XL_ODR_12Hz5  = 1,
  LSM6DSO_XL_ODR_26Hz   = 2,
  LSM6DSO_XL_ODR_52Hz   = 3,
  LSM6DSO_XL_ODR_104Hz  = 4,
  LSM6DSO_XL_ODR_208Hz  = 5,
  LSM6DSO_XL_ODR_417Hz  = 6,
  LSM6DSO_XL_ODR_833Hz  = 7,
  LSM6DSO_XL_ODR_1667Hz = 8,
  LSM6DSO_XL_ODR_3333Hz = 9,
  LSM6DSO_XL_ODR_6667Hz = 10,
  LSM6DSO_XL_ODR_1Hz6   = 11, /* (low power only) */
} LSM6DSO_ODR_XL;

typedef enum LSM6DSO_FS_G_t {
  LSM6DSO_250dps   = 0,
  LSM6DSO_125dps   = 1,
  LSM6DSO_500dps   = 2,
  LSM6DSO_1000dps  = 4,
  LSM6DSO_2000dps  = 6,
} LSM6DSO_FS_G;

typedef enum LSM6DSO_ODR_G_t {
  LSM6DSO_GY_ODR_OFF    = 0,
  LSM6DSO_GY_ODR_12Hz5  = 1,
  LSM6DSO_GY_ODR_26Hz   = 2,
  LSM6DSO_GY_ODR_52Hz   = 3,
  LSM6DSO_GY_ODR_104Hz  = 4,
  LSM6DSO_GY_ODR_208Hz  = 5,
  LSM6DSO_GY_ODR_417Hz  = 6,
  LSM6DSO_GY_ODR_833Hz  = 7,
  LSM6DSO_GY_ODR_1667Hz = 8,
  LSM6DSO_GY_ODR_3333Hz = 9,
  LSM6DSO_GY_ODR_6667Hz = 10,
} LSM6DSO_ODR_G;

typedef enum LSM6DSO_Usr_Off_W_t {
  LSM6DSO_LSb_1mg  = 0,
  LSM6DSO_LSb_16mg = 1,
} LSM6DSO_Usr_Off_W;

typedef enum LSM6DSO_XL_HM_Mode_t {
  LSM6DSO_HIGH_PERFORMANCE_MD  = 0,
  LSM6DSO_LOW_NORMAL_POWER_MD  = 1,
  LSM6DSO_ULTRA_LOW_POWER_MD   = 2,
} LSM6DSO_XL_HM_Mode;

typedef enum LSM6DSO_G_HM_Mode_t {
  LSM6DSO_GY_HIGH_PERFORMANCE  = 0,
  LSM6DSO_GY_NORMAL            = 1,
} LSM6DSO_G_HM_Mode;

typedef enum LSM6DSO_Rounding_t {
  LSM6DSO_NO_ROUND      = 0,
  LSM6DSO_ROUND_XL      = 1,
  LSM6DSO_ROUND_GY      = 2,
  LSM6DSO_ROUND_GY_XL   = 3,
} LSM6DSO_Rounding;

typedef enum LSM6DSO_Reg_Access_t {
  LSM6DSO_USER_BANK           = 0,
  LSM6DSO_SENSOR_HUB_BANK     = 1,
  LSM6DSO_EMBEDDED_FUNC_BANK  = 2,
} LSM6DSO_Reg_Access;

typedef enum LSM6DSO_DataReady_Pulsed_t {
  LSM6DSO_DRDY_LATCHED = 0,
  LSM6DSO_DRDY_PULSED  = 1,
} LSM6DSO_DataReady_Pulsed;

typedef enum LSM6DSO_ST_XL_t {
  LSM6DSO_XL_ST_DISABLE  = 0,
  LSM6DSO_XL_ST_POSITIVE = 1,
  LSM6DSO_XL_ST_NEGATIVE = 2,
} LSM6DSO_ST_XL;

typedef enum LSM6DSO_ST_G_t {
  LSM6DSO_GY_ST_DISABLE  = 0,
  LSM6DSO_GY_ST_POSITIVE = 1,
  LSM6DSO_GY_ST_NEGATIVE = 3,
} LSM6DSO_ST_G;

typedef enum LSM6DSO_fType_t{
  LSM6DSO_ULTRA_LIGHT  = 0,
  LSM6DSO_VERY_LIGHT   = 1,
  LSM6DSO_LIGHT        = 2,
  LSM6DSO_MEDIUM       = 3,
  LSM6DSO_STRONG       = 4, /* not available for data rate > 1k670Hz */
  LSM6DSO_VERY_STRONG  = 5, /* not available for data rate > 1k670Hz */
  LSM6DSO_AGGRESSIVE   = 6, /* not available for data rate > 1k670Hz */
  LSM6DSO_XTREME       = 7, /* not available for data rate > 1k670Hz */
} LSM6DSO_fType_t;

typedef enum LSM6DSO_HP_Slope_XL_En_t {
  LSM6DSO_HP_PATH_DISABLE_ON_OUT    = 0x00,
  LSM6DSO_SLOPE_ODR_DIV_4           = 0x10,
  LSM6DSO_HP_ODR_DIV_10             = 0x11,
  LSM6DSO_HP_ODR_DIV_20             = 0x12,
  LSM6DSO_HP_ODR_DIV_45             = 0x13,
  LSM6DSO_HP_ODR_DIV_100            = 0x14,
  LSM6DSO_HP_ODR_DIV_200            = 0x15,
  LSM6DSO_HP_ODR_DIV_400            = 0x16,
  LSM6DSO_HP_ODR_DIV_800            = 0x17,
  LSM6DSO_HP_REF_MD_ODR_DIV_10      = 0x31,
  LSM6DSO_HP_REF_MD_ODR_DIV_20      = 0x32,
  LSM6DSO_HP_REF_MD_ODR_DIV_45      = 0x33,
  LSM6DSO_HP_REF_MD_ODR_DIV_100     = 0x34,
  LSM6DSO_HP_REF_MD_ODR_DIV_200     = 0x35,
  LSM6DSO_HP_REF_MD_ODR_DIV_400     = 0x36,
  LSM6DSO_HP_REF_MD_ODR_DIV_800     = 0x37,
  LSM6DSO_LP_ODR_DIV_10             = 0x01,
  LSM6DSO_LP_ODR_DIV_20             = 0x02,
  LSM6DSO_LP_ODR_DIV_45             = 0x03,
  LSM6DSO_LP_ODR_DIV_100            = 0x04,
  LSM6DSO_LP_ODR_DIV_200            = 0x05,
  LSM6DSO_LP_ODR_DIV_400            = 0x06,
  LSM6DSO_LP_ODR_DIV_800            = 0x07,
} LSM6DSO_HP_Slope_XL_En;

typedef enum LSM6DSO_Slope_Fds_t {
  LSM6DSO_USE_SLOPE = 0,
  LSM6DSO_USE_HPF   = 1,
} LSM6DSO_Slope_Fds;

typedef enum LSM6DSO_HPM_G_t {
  LSM6DSO_HP_FILTER_NONE     = 0x00,
  LSM6DSO_HP_FILTER_16mHz    = 0x80,
  LSM6DSO_HP_FILTER_65mHz    = 0x81,
  LSM6DSO_HP_FILTER_260mHz   = 0x82,
  LSM6DSO_HP_FILTER_1Hz04    = 0x83,
} LSM6DSO_HPM_G;

typedef enum LSM6DSO_Ois_PU_Dis_t {
  LSM6DSO_AUX_PULL_UP_DISC       = 0,
  LSM6DSO_AUX_PULL_UP_CONNECT    = 1,
} LSM6DSO_Ois_PU_Dis;

typedef enum LSM6DSO_Ois_On_t {
  LSM6DSO_AUX_ON                    = 1,
  LSM6DSO_AUX_ON_BY_AUX_INTERFACE   = 0,
} LSM6DSO_Ois_On;

typedef enum LSM6DSO_XL_FS_Mode_t {
  LSM6DSO_USE_SAME_XL_FS        = 0,
  LSM6DSO_USE_DIFFERENT_XL_FS   = 1,
} LSM6DSO_XL_FS_Mode;

typedef enum LSM6DSO_St_XL_Ois_t {
  LSM6DSO_AUX_XL_DISABLE = 0,
  LSM6DSO_AUX_XL_POS     = 1,
  LSM6DSO_AUX_XL_NEG     = 2,
} LSM6DSO_St_XL_Ois;

typedef enum LSM6DSO_Den_LH_Ois_t {
  LSM6DSO_AUX_DEN_ACTIVE_LOW     = 0,
  LSM6DSO_AUX_DEN_ACTIVE_HIGH    = 1,
} LSM6DSO_Den_LH_Ois;

typedef enum LSM6DSO_Lvl2_Ois_t {
  LSM6DSO_AUX_DEN_DISABLE         = 0,
  LSM6DSO_AUX_DEN_LEVEL_LATCH     = 3,
  LSM6DSO_AUX_DEN_LEVEL_TRIG      = 2,
} LSM6DSO_Lvl2_Ois;

typedef enum LSM6DSO_Ois_En_SPI2_t {
  LSM6DSO_AUX_DISABLE  = 0,
  LSM6DSO_MODE_3_GY    = 1,
  LSM6DSO_MODE_4_GY_XL = 3,
} LSM6DSO_Ois_En_SPI2;

typedef enum LSM6DSO_FS_G_Ois_t {
  LSM6DSO_250dps_AUX  = 0,
  LSM6DSO_125dps_AUX  = 1,
  LSM6DSO_500dps_AUX  = 2,
  LSM6DSO_1000dps_AUX = 4,
  LSM6DSO_2000dps_AUX = 6,
} LSM6DSO_FS_G_Ois;

typedef enum LSM6DSO_Sim_Ois_t {
  LSM6DSO_AUX_SPI_4_WIRE = 0,
  LSM6DSO_AUX_SPI_3_WIRE = 1,
} LSM6DSO_Sim_Ois;

typedef enum LSM6DSO_fType_Ois_t {
  LSM6DSO_351Hz39 = 0,
  LSM6DSO_236Hz63 = 1,
  LSM6DSO_172Hz70 = 2,
  LSM6DSO_937Hz91 = 3,
} LSM6DSO_fType_Ois;

typedef enum LSM6DSO_HPM_Ois_t {
  LSM6DSO_AUX_HP_DISABLE = 0x00,
  LSM6DSO_AUX_HP_Hz016   = 0x10,
  LSM6DSO_AUX_HP_Hz065   = 0x11,
  LSM6DSO_AUX_HP_Hz260   = 0x12,
  LSM6DSO_AUX_HP_1Hz040  = 0x13,
} LSM6DSO_HPM_Ois;

typedef enum LSM6DSO_ST_Ois_ClampDis_t {
  LSM6DSO_ENABLE_CLAMP  = 0,
  LSM6DSO_DISABLE_CLAMP = 1,
} LSM6DSO_ST_Ois_ClampDis;

typedef enum LSM6DSO_ST_Ois_t {
  LSM6DSO_AUX_GY_DISABLE = 0,
  LSM6DSO_AUX_GY_POS     = 1,
  LSM6DSO_AUX_GY_NEG     = 3,
} LSM6DSO_ST_Ois;

typedef enum LSM6DSO_Filter_XL_Conf_Ois_t {
  LSM6DSO_289Hz = 0,
  LSM6DSO_258Hz = 1,
  LSM6DSO_120Hz = 2,
  LSM6DSO_65Hz2 = 3,
  LSM6DSO_33Hz2 = 4,
  LSM6DSO_16Hz6 = 5,
  LSM6DSO_8Hz30 = 6,
  LSM6DSO_4Hz15 = 7,
} LSM6DSO_Filter_XL_Conf_Ois;

typedef enum LSM6DSO_FS_XL_Ois_t {
  LSM6DSO_AUX_2g  = 0,
  LSM6DSO_AUX_16g = 1,
  LSM6DSO_AUX_4g  = 2,
  LSM6DSO_AUX_8g  = 3,
} LSM6DSO_FS_XL_Ois;

typedef enum LSM6DSO_SDO_PU_En_t {
  LSM6DSO_PULL_UP_DISC       = 0,
  LSM6DSO_PULL_UP_CONNECT    = 1,
} LSM6DSO_SDO_PU_En;

typedef enum LSM6DSO_Sim_t {
  LSM6DSO_SPI_4_WIRE = 0,
  LSM6DSO_SPI_3_WIRE = 1,
} LSM6DSO_Sim;

typedef enum LSM6DSO_I2C_Disable_t {
  LSM6DSO_I2C_ENABLE  = 0,
  LSM6DSO_I2C_DISABLE = 1,
} LSM6DSO_I2C_Disable;

typedef enum LSM6DSO_I3C_Disable_t {
  LSM6DSO_I3C_DISABLE         = 0x80,
  LSM6DSO_I3C_ENABLE_T_50us   = 0x00,
  LSM6DSO_I3C_ENABLE_T_2us    = 0x01,
  LSM6DSO_I3C_ENABLE_T_1ms    = 0x02,
  LSM6DSO_I3C_ENABLE_T_25ms   = 0x03,
} LSM6DSO_I3C_Disable;

typedef enum LSM6DSO_Int1_PD_En_t {
  LSM6DSO_PULL_DOWN_DISC       = 0,
  LSM6DSO_PULL_DOWN_CONNECT    = 1,
} LSM6DSO_Int1_PD_En;

typedef enum LSM6DSO_PP_OD_t {
  LSM6DSO_PUSH_PULL   = 0,
  LSM6DSO_OPEN_DRAIN  = 1,
} LSM6DSO_PP_OD;

typedef enum LSM6DSO_HL_Active_t {
  LSM6DSO_ACTIVE_HIGH = 0,
  LSM6DSO_ACTIVE_LOW  = 1,
} LSM6DSO_HL_Active;

typedef enum LSM6DSO_LIR_t {
  LSM6DSO_ALL_INT_PULSED            = 0,
  LSM6DSO_BASE_LATCHED_EMB_PULSED   = 1,
  LSM6DSO_BASE_PULSED_EMB_LATCHED   = 2,
  LSM6DSO_ALL_INT_LATCHED           = 3,
} LSM6DSO_LIR;

typedef enum LSM6DSO_Wake_Ths_W_t {
  LSM6DSO_LSb_FS_DIV_64       = 0,
  LSM6DSO_LSb_FS_DIV_256      = 1,
} LSM6DSO_Wake_Ths_W;

typedef enum LSM6DSO_Sleep_Status_On_Int_t {
  LSM6DSO_DRIVE_SLEEP_CHG_EVENT = 0,
  LSM6DSO_DRIVE_SLEEP_STATUS    = 1,
} LSM6DSO_Sleep_Status_On_Int;

typedef enum LSM6DSO_InAct_En_t {
  LSM6DSO_XL_AND_GY_NOT_AFFECTED      = 0,
  LSM6DSO_XL_12Hz5_GY_NOT_AFFECTED    = 1,
  LSM6DSO_XL_12Hz5_GY_SLEEP           = 2,
  LSM6DSO_XL_12Hz5_GY_PD              = 3,
} LSM6DSO_InAct_En;

typedef enum LSM6DSO_Tap_Priority_t {
  LSM6DSO_XYZ = 0,
  LSM6DSO_YXZ = 1,
  LSM6DSO_XZY = 2,
  LSM6DSO_ZYX = 3,
  LSM6DSO_YZX = 5,
  LSM6DSO_ZXY = 6,
} LSM6DSO_Tap_Priority;

typedef enum LSM6DSO_Single_Double_Tap_t {
  LSM6DSO_ONLY_SINGLE = 0,
  LSM6DSO_BOTH_SINGLE_DOUBLE = 1,
} LSM6DSO_Single_Double_Tap;

typedef enum LSM6DSO_6D_Ths_t {
  LSM6DSO_DEG_80  = 0,
  LSM6DSO_DEG_70  = 1,
  LSM6DSO_DEG_60  = 2,
  LSM6DSO_DEG_50  = 3,
} LSM6DSO_6D_Ths;

typedef enum LSM6DSO_FF_Ths_t {
  LSM6DSO_FF_TSH_156mg = 0,
  LSM6DSO_FF_TSH_219mg = 1,
  LSM6DSO_FF_TSH_250mg = 2,
  LSM6DSO_FF_TSH_312mg = 3,
  LSM6DSO_FF_TSH_344mg = 4,
  LSM6DSO_FF_TSH_406mg = 5,
  LSM6DSO_FF_TSH_469mg = 6,
  LSM6DSO_FF_TSH_500mg = 7,
} LSM6DSO_FF_Ths;

typedef enum LSM6DSO_UnCoPtr_Rate_t {
  LSM6DSO_CMP_DISABLE  = 0x00,
  LSM6DSO_CMP_ALWAYS   = 0x04,
  LSM6DSO_CMP_8_TO_1   = 0x05,
  LSM6DSO_CMP_16_TO_1  = 0x06,
  LSM6DSO_CMP_32_TO_1  = 0x07,
} LSM6DSO_UnCoPtr_Rate;

typedef enum LSM6DSO_BDR_XL_t {
  LSM6DSO_XL_NOT_BATCHED       =  0,
  LSM6DSO_XL_BATCHED_AT_12Hz5   =  1,
  LSM6DSO_XL_BATCHED_AT_26Hz    =  2,
  LSM6DSO_XL_BATCHED_AT_52Hz    =  3,
  LSM6DSO_XL_BATCHED_AT_104Hz   =  4,
  LSM6DSO_XL_BATCHED_AT_208Hz   =  5,
  LSM6DSO_XL_BATCHED_AT_417Hz   =  6,
  LSM6DSO_XL_BATCHED_AT_833Hz   =  7,
  LSM6DSO_XL_BATCHED_AT_1667Hz  =  8,
  LSM6DSO_XL_BATCHED_AT_3333Hz  =  9,
  LSM6DSO_XL_BATCHED_AT_6667Hz  = 10,
  LSM6DSO_XL_BATCHED_AT_6Hz5    = 11,
} LSM6DSO_BDR_XL;

typedef enum LSM6DSO_BDR_Gy_t {
  LSM6DSO_GY_NOT_BATCHED         = 0,
  LSM6DSO_GY_BATCHED_AT_12Hz5    = 1,
  LSM6DSO_GY_BATCHED_AT_26Hz     = 2,
  LSM6DSO_GY_BATCHED_AT_52Hz     = 3,
  LSM6DSO_GY_BATCHED_AT_104Hz    = 4,
  LSM6DSO_GY_BATCHED_AT_208Hz    = 5,
  LSM6DSO_GY_BATCHED_AT_417Hz    = 6,
  LSM6DSO_GY_BATCHED_AT_833Hz    = 7,
  LSM6DSO_GY_BATCHED_AT_1667Hz   = 8,
  LSM6DSO_GY_BATCHED_AT_3333Hz   = 9,
  LSM6DSO_GY_BATCHED_AT_6667Hz   = 10,
  LSM6DSO_GY_BATCHED_AT_6Hz5     = 11,
} LSM6DSO_BDR_Gy;

typedef enum LSM6DSO_FIFO_Mode_t {
  LSM6DSO_BYPASS_MODE             = 0,
  LSM6DSO_FIFO_MODE               = 1,
  LSM6DSO_STREAM_TO_FIFO_MODE     = 3,
  LSM6DSO_BYPASS_TO_STREAM_MODE   = 4,
  LSM6DSO_STREAM_MODE             = 6,
  LSM6DSO_BYPASS_TO_FIFO_MODE     = 7,
} LSM6DSO_FIFO_Mode;

typedef enum LSM6DSO_ODR_Temp_Batch_t {
  LSM6DSO_TEMP_NOT_BATCHED        = 0,
  LSM6DSO_TEMP_BATCHED_AT_1Hz6    = 1,
  LSM6DSO_TEMP_BATCHED_AT_12Hz5   = 2,
  LSM6DSO_TEMP_BATCHED_AT_52Hz    = 3,
} LSM6DSO_ODR_Temp_Batch;

typedef enum LSM6DSO_ODR_TS_Batch_t {
  LSM6DSO_NO_DECIMATION = 0,
  LSM6DSO_DEC_1         = 1,
  LSM6DSO_DEC_8         = 2,
  LSM6DSO_DEC_32        = 3,
} LSM6DSO_ODR_TS_Batch;

typedef enum LSM6DSO_Trig_Counter_BDR_t {
  LSM6DSO_XL_BATCH_EVENT   = 0,
  LSM6DSO_GYRO_BATCH_EVENT = 1,
} LSM6DSO_Trig_Counter_BDR;

typedef enum LSM6DSO_FIFO_Tag_t {
  LSM6DSO_GYRO_NC_TAG    = 1,
  LSM6DSO_XL_NC_TAG,
  LSM6DSO_TEMPERATURE_TAG,
  LSM6DSO_TIMESTAMP_TAG,
  LSM6DSO_CFG_CHANGE_TAG,
  LSM6DSO_XL_NC_T_2_TAG,
  LSM6DSO_XL_NC_T_1_TAG,
  LSM6DSO_XL_2XC_TAG,
  LSM6DSO_XL_3XC_TAG,
  LSM6DSO_GYRO_NC_T_2_TAG,
  LSM6DSO_GYRO_NC_T_1_TAG,
  LSM6DSO_GYRO_2XC_TAG,
  LSM6DSO_GYRO_3XC_TAG,
  LSM6DSO_SENSORHUB_SLAVE0_TAG,
  LSM6DSO_SENSORHUB_SLAVE1_TAG,
  LSM6DSO_SENSORHUB_SLAVE2_TAG,
  LSM6DSO_SENSORHUB_SLAVE3_TAG,
  LSM6DSO_STEP_CPUNTER_TAG,
  LSM6DSO_GAME_ROTATION_TAG,
  LSM6DSO_GEOMAG_ROTATION_TAG,
  LSM6DSO_ROTATION_TAG,
  LSM6DSO_SENSORHUB_NACK_TAG  = 0x19,
} LSM6DSO_FIFO_Tag;

typedef enum LSM6DSO_Den_Mode_t {
  LSM6DSO_DEN_DISABLE    = 0,
  LSM6DSO_LEVEL_FIFO     = 6,
  LSM6DSO_LEVEL_LETCHED  = 3,
  LSM6DSO_LEVEL_TRIGGER  = 2,
  LSM6DSO_EDGE_TRIGGER   = 4,
} LSM6DSO_Den_Mode;

typedef enum LSM6DSO_Den_LH_t {
  LSM6DSO_DEN_ACT_LOW  = 0,
  LSM6DSO_DEN_ACT_HIGH = 1,
} LSM6DSO_Den_LH;

typedef enum LSM6DSO_Den_XL_G_t{
  LSM6DSO_STAMP_IN_GY_DATA     = 0,
  LSM6DSO_STAMP_IN_XL_DATA     = 1,
  LSM6DSO_STAMP_IN_GY_XL_DATA  = 2,
} LSM6DSO_Den_XL_G;

typedef enum LSM6DSO_Pedo_MD_t {
  LSM6DSO_PEDO_BASE_MODE            = 0x00,
  LSM6DSO_FALSE_STEP_REJ            = 0x10,
  LSM6DSO_FALSE_STEP_REJ_ADV_MODE   = 0x30,
} LSM6DSO_Pedo_MD;

typedef enum LSM6DSO_Carry_Count_En_t {
  LSM6DSO_EVERY_STEP     = 0,
  LSM6DSO_COUNT_OVERFLOW = 1,
} LSM6DSO_Carry_Count_En;

typedef enum LSM6DSO_Mag_Z_Axis_t {
  LSM6DSO_Z_EQ_Y     = 0,
  LSM6DSO_Z_EQ_MIN_Y = 1,
  LSM6DSO_Z_EQ_X     = 2,
  LSM6DSO_Z_EQ_MIN_X = 3,
  LSM6DSO_Z_EQ_MIN_Z = 4,
  LSM6DSO_Z_EQ_Z     = 5,
} LSM6DSO_Mag_Z_Axis;

typedef enum LSM6DSO_Mag_Y_Axis_t {
  LSM6DSO_Y_EQ_Y     = 0,
  LSM6DSO_Y_EQ_MIN_Y = 1,
  LSM6DSO_Y_EQ_X     = 2,
  LSM6DSO_Y_EQ_MIN_X = 3,
  LSM6DSO_Y_EQ_MIN_Z = 4,
  LSM6DSO_Y_EQ_Z     = 5,
} LSM6DSO_Mag_Y_Axis;

typedef enum LSM6DSO_Mag_X_Axis_t {
  LSM6DSO_X_EQ_Y     = 0,
  LSM6DSO_X_EQ_MIN_Y = 1,
  LSM6DSO_X_EQ_X     = 2,
  LSM6DSO_X_EQ_MIN_X = 3,
  LSM6DSO_X_EQ_MIN_Z = 4,
  LSM6DSO_X_EQ_Z     = 5,
} LSM6DSO_Mag_X_Axis;

typedef struct LSM6DSO_Emb_FSM_Enable_t {
    LSM6DSO_FSM_Enable_A          fsm_enable_a;
    LSM6DSO_FSM_Enable_B          fsm_enable_b;
} LSM6DSO_Emb_FSM_Enable;

typedef enum LSM6DSO_FSM_LC_Clr_t {
  LSM6DSO_LC_NORMAL     = 0,
  LSM6DSO_LC_CLEAR      = 1,
  LSM6DSO_LC_CLEAR_DONE = 2,
} LSM6DSO_FSM_LC_Clr;

typedef struct LSM6DSO_FSM_Out_t {
    LSM6DSO_FSM_Outs1    fsm_outs1;
    LSM6DSO_FSM_Outs2    fsm_outs2;
    LSM6DSO_FSM_Outs3    fsm_outs3;
    LSM6DSO_FSM_Outs4    fsm_outs4;
    LSM6DSO_FSM_Outs5    fsm_outs5;
    LSM6DSO_FSM_Outs6    fsm_outs6;
    LSM6DSO_FSM_Outs7    fsm_outs7;
    LSM6DSO_FSM_Outs8    fsm_outs8;
    LSM6DSO_FSM_Outs9    fsm_outs9;
    LSM6DSO_FSM_Outs10   fsm_outs10;
    LSM6DSO_FSM_Outs11   fsm_outs11;
    LSM6DSO_FSM_Outs12   fsm_outs12;
    LSM6DSO_FSM_Outs13   fsm_outs13;
    LSM6DSO_FSM_Outs14   fsm_outs14;
    LSM6DSO_FSM_Outs15   fsm_outs15;
    LSM6DSO_FSM_Outs16   fsm_outs16;
} LSM6DSO_FSM_Out;

typedef enum LSM6DSO_FSM_ODR_t {
  LSM6DSO_ODR_FSM_12Hz5 = 0,
  LSM6DSO_ODR_FSM_26Hz  = 1,
  LSM6DSO_ODR_FSM_52Hz  = 2,
  LSM6DSO_ODR_FSM_104Hz = 3,
} LSM6DSO_FSM_ODR;

typedef enum LSM6DSO_Aux_Sens_On_t {
  LSM6DSO_SLV_0       = 0,
  LSM6DSO_SLV_0_1     = 1,
  LSM6DSO_SLV_0_1_2   = 2,
  LSM6DSO_SLV_0_1_2_3 = 3,
} LSM6DSO_Aux_Sens_On;

typedef enum LSM6DSO_Shub_PU_En_t {
  LSM6DSO_EXT_PULL_UP      = 0,
  LSM6DSO_INTERNAL_PULL_UP = 1,
} LSM6DSO_Shub_PU_En;

typedef enum LSM6DSO_Start_Config_t {
  LSM6DSO_EXT_ON_INT2_PIN = 0,
  LSM6DSO_XL_GY_DRDY      = 1,
} LSM6DSO_Start_Config;

typedef enum LSM6DSO_Write_Once_t {
  LSM6DSO_EACH_SH_CYCLE    = 0,
  LSM6DSO_ONLY_FIRST_CYCLE = 1,
} LSM6DSO_Write_Once;

typedef enum LSM6DSO_Shub_ODR_t {
  LSM6DSO_SH_ODR_104Hz = 0,
  LSM6DSO_SH_ODR_52Hz  = 1,
  LSM6DSO_SH_ODR_26Hz  = 2,
  LSM6DSO_SH_ODR_13Hz  = 3,
} LSM6DSO_Shub_ODR;

typedef struct LSM6DSO_Sh_Cfg_Write_t {
  uint8_t   slv0_add;
  uint8_t   slv0_subadd;
  uint8_t   slv0_data;
} LSM6DSO_Sh_Cfg_Write;

typedef struct LSM6DSO_Sh_Cfg_Read_t {
  uint8_t   slv_add;
  uint8_t   slv_subadd;
  uint8_t   slv_len;
} LSM6DSO_Sh_Cfg_Read;

typedef struct LSM6DSO_ID_t {
  uint8_t ui;
  uint8_t aux;
} LSM6DSO_ID;

typedef struct LSM6DSO_Bus_Mode_t {
  enum {
    LSM6DSO_SEL_BY_HW   = 0x00, /* bus mode select by HW (SPI 3W disable) */
    LSM6DSO_SPI_4W      = 0x06, /* Only SPI: SDO / SDI separated pins */
    LSM6DSO_SPI_3W      = 0x07, /* Only SPI: SDO / SDI share the same pin */
    LSM6DSO_I2C         = 0x04, /* Only I2C */
    LSM6DSO_I3C_T_50us  = 0x02, /* I3C: available time equal to 50 μs */
    LSM6DSO_I3C_T_2us   = 0x12, /* I3C: available time equal to 2 μs */
    LSM6DSO_I3C_T_1ms   = 0x22, /* I3C: available time equal to 1 ms */
    LSM6DSO_I3C_T_25ms  = 0x32, /* I3C: available time equal to 25 ms */
  } ui_bus_md;
  enum {
    LSM6DSO_SPI_4W_AUX  = 0x00,
    LSM6DSO_SPI_3W_AUX  = 0x01,
  } aux_bus_md;
} LSM6DSO_Bus_Mode;

typedef enum LSM6DSO_Init_Set_t {
  LSM6DSO_DRV_RDY   = 0x00, /* Initialize the device for driver usage */
  LSM6DSO_BOOT      = 0x01, /* Restore calib. param. ( it takes 10ms ) */
  LSM6DSO_RESET     = 0x02, /* Reset configuration registers */
  LSM6DSO_FIFO_COMP = 0x04, /* FIFO compression initialization request. */
  LSM6DSO_FSM       = 0x08, /* Finite State Machine initialization request */
  LSM6DSO_PEDO      = 0x20, /* Pedometer algo initialization request. */
  LSM6DSO_TILT      = 0x40, /* Tilt algo initialization request */
  LSM6DSO_SMOTION   = 0x80, /* Significant Motion initialization request */
} LSM6DSO_Init_Set;

typedef struct LSM6DSO_Status_t {
  uint8_t sw_reset           : 1; /* Restoring configuration registers */
  uint8_t boot               : 1; /* Restoring calibration parameters */
  uint8_t drdy_xl            : 1; /* Accelerometer data ready */
  uint8_t drdy_g             : 1; /* Gyroscope data ready */
  uint8_t drdy_temp          : 1; /* Temperature data ready */
  uint8_t ois_drdy_xl        : 1; /* Accelerometer data ready on OIS */
  uint8_t ois_drdy_g         : 1; /* Gyroscope data ready on OIS */
  uint8_t ois_gyro_settling  : 1; /* Gyroscope is in the settling phase */
} LSM6DSO_Status;

typedef struct LSM6DSO_Pin_Conf_t {
  uint8_t sdo_sa0_pull_up     : 1; /* 1 = pull-up on SDO/SA0 pin */
  uint8_t aux_sdo_ocs_pull_up : 1; /* 1 = pull-up on OCS_Aux/SDO_Aux pins */
  uint8_t int1_int2_push_pull : 1; /* 1 = push-pull / 0 = open-drain*/
  uint8_t int1_pull_down      : 1; /* 1 = pull-down always disabled (0=auto) */
} LSM6DSO_Pin_Conf;

typedef struct LSM6DSO_Int_Mode_t {
  uint8_t active_low   : 1; /* 1 = active low / 0 = active high */
  uint8_t base_latched : 1; /* base functions are: FF, WU, 6D, Tap, Act/Inac */
  uint8_t emb_latched  : 1; /* emb functions are: Pedo, Tilt, SMot, Timestamp */
} LSM6DSO_Int_Mode;

typedef struct LSM6DSO_Pin_Int1_Route_t {
  uint8_t drdy_xl       : 1; /* Accelerometer data ready */
  uint8_t drdy_g        : 1; /* Gyroscope data ready */
  uint8_t drdy_temp     : 1; /* Temperature data ready (1 = int2 pin disable) */
  uint8_t boot          : 1; /* Restoring calibration parameters */
  uint8_t fifo_th       : 1; /* FIFO threshold reached */
  uint8_t fifo_ovr      : 1; /* FIFO overrun */
  uint8_t fifo_full     : 1; /* FIFO full */
  uint8_t fifo_bdr      : 1; /* FIFO Batch counter threshold reached */
  uint8_t den_flag      : 1; /* external trigger level recognition (DEN) */
  uint8_t sh_endop      : 1; /* sensor hub end operation */
  uint8_t timestamp     : 1; /* timestamp overflow (1 = int2 pin disable) */
  uint8_t six_d         : 1; /* orientation change (6D/4D detection) */
  uint8_t double_tap    : 1; /* double-tap event */
  uint8_t free_fall     : 1; /* free fall event */
  uint8_t wake_up       : 1; /* wake up event */
  uint8_t single_tap    : 1; /* single-tap event */
  uint8_t sleep_change  : 1; /* Act/Inact (or Vice-versa) status changed */
  uint8_t step_detector : 1; /* Step detected */
  uint8_t tilt          : 1; /* Relative tilt event detected */
  uint8_t sig_mot       : 1; /* "significant motion" event detected */
  uint8_t fsm_lc        : 1; /* fsm long counter timeout interrupt event */
  uint8_t fsm1          : 1; /* fsm 1 interrupt event */
  uint8_t fsm2          : 1; /* fsm 2 interrupt event */
  uint8_t fsm3          : 1; /* fsm 3 interrupt event */
  uint8_t fsm4          : 1; /* fsm 4 interrupt event */
  uint8_t fsm5          : 1; /* fsm 5 interrupt event */
  uint8_t fsm6          : 1; /* fsm 6 interrupt event */
  uint8_t fsm7          : 1; /* fsm 7 interrupt event */
  uint8_t fsm8          : 1; /* fsm 8 interrupt event */
  uint8_t fsm9          : 1; /* fsm 9 interrupt event */
  uint8_t fsm10         : 1; /* fsm 10 interrupt event */
  uint8_t fsm11         : 1; /* fsm 11 interrupt event */
  uint8_t fsm12         : 1; /* fsm 12 interrupt event */
  uint8_t fsm13         : 1; /* fsm 13 interrupt event */
  uint8_t fsm14         : 1; /* fsm 14 interrupt event */
  uint8_t fsm15         : 1; /* fsm 15 interrupt event */
  uint8_t fsm16         : 1; /* fsm 16 interrupt event */
  uint8_t mlc1          : 1; /* mlc 1 interrupt event */
  uint8_t mlc2          : 1; /* mlc 2 interrupt event */
  uint8_t mlc3          : 1; /* mlc 3 interrupt event */
  uint8_t mlc4          : 1; /* mlc 4 interrupt event */
  uint8_t mlc5          : 1; /* mlc 5 interrupt event */
  uint8_t mlc6          : 1; /* mlc 6 interrupt event */
  uint8_t mlc7          : 1; /* mlc 7 interrupt event */
  uint8_t mlc8          : 1; /* mlc 8 interrupt event */
} LSM6DSO_Pin_Int1_Route;

typedef struct LSM6DSO_Pin_Int2_Route_t {
  uint8_t drdy_ois      : 1; /* OIS chain data ready */
  uint8_t drdy_xl       : 1; /* Accelerometer data ready */
  uint8_t drdy_g        : 1; /* Gyroscope data ready */
  uint8_t drdy_temp     : 1; /* Temperature data ready */
  uint8_t fifo_th       : 1; /* FIFO threshold reached */
  uint8_t fifo_ovr      : 1; /* FIFO overrun */
  uint8_t fifo_full     : 1; /* FIFO full */
  uint8_t fifo_bdr      : 1; /* FIFO Batch counter threshold reached */
  uint8_t timestamp     : 1; /* timestamp overflow */
  uint8_t six_d         : 1; /* orientation change (6D/4D detection) */
  uint8_t double_tap    : 1; /* double-tap event */
  uint8_t free_fall     : 1; /* free fall event */
  uint8_t wake_up       : 1; /* wake up event */
  uint8_t single_tap    : 1; /* single-tap event */
  uint8_t sleep_change  : 1; /* Act/Inact (or Vice-versa) status changed */
  uint8_t step_detector : 1; /* Step detected */
  uint8_t tilt          : 1; /* Relative tilt event detected */
  uint8_t sig_mot       : 1; /* "significant motion" event detected */
  uint8_t fsm_lc        : 1; /* fsm long counter timeout interrupt event */
  uint8_t fsm1          : 1; /* fsm 1 interrupt event */
  uint8_t fsm2          : 1; /* fsm 2 interrupt event */
  uint8_t fsm3          : 1; /* fsm 3 interrupt event */
  uint8_t fsm4          : 1; /* fsm 4 interrupt event */
  uint8_t fsm5          : 1; /* fsm 5 interrupt event */
  uint8_t fsm6          : 1; /* fsm 6 interrupt event */
  uint8_t fsm7          : 1; /* fsm 7 interrupt event */
  uint8_t fsm8          : 1; /* fsm 8 interrupt event */
  uint8_t fsm9          : 1; /* fsm 9 interrupt event */
  uint8_t fsm10         : 1; /* fsm 10 interrupt event */
  uint8_t fsm11         : 1; /* fsm 11 interrupt event */
  uint8_t fsm12         : 1; /* fsm 12 interrupt event */
  uint8_t fsm13         : 1; /* fsm 13 interrupt event */
  uint8_t fsm14         : 1; /* fsm 14 interrupt event */
  uint8_t fsm15         : 1; /* fsm 15 interrupt event */
  uint8_t fsm16         : 1; /* fsm 16 interrupt event */
  uint8_t mlc1          : 1; /* mlc 1 interrupt event */
  uint8_t mlc2          : 1; /* mlc 2 interrupt event */
  uint8_t mlc3          : 1; /* mlc 3 interrupt event */
  uint8_t mlc4          : 1; /* mlc 4 interrupt event */
  uint8_t mlc5          : 1; /* mlc 5 interrupt event */
  uint8_t mlc6          : 1; /* mlc 6 interrupt event */
  uint8_t mlc7          : 1; /* mlc 7 interrupt event */
  uint8_t mlc8          : 1; /* mlc 8 interrupt event */
} LSM6DSO_Pin_Int2_Route;

typedef struct LSM6DSO_All_Sources_t{
  uint8_t drdy_xl          :  1; /* Accelerometer data ready */
  uint8_t drdy_g           :  1; /* Gyroscope data ready */
  uint8_t drdy_temp        :  1; /* Temperature data ready */
  uint8_t den_flag         :  1; /* external trigger level recognition (DEN) */
  uint8_t timestamp        :  1; /* timestamp overflow (1 = int2 pin disable) */
  uint8_t free_fall        :  1; /* free fall event */
  uint8_t wake_up          :  1; /* wake up event */
  uint8_t wake_up_z        :  1; /* wake up on Z axis event */
  uint8_t wake_up_y        :  1; /* wake up on Y axis event */
  uint8_t wake_up_x        :  1; /* wake up on X axis event */
  uint8_t single_tap       :  1; /* single-tap event */
  uint8_t double_tap       :  1; /* double-tap event */
  uint8_t tap_z            :  1; /* single-tap on Z axis event */
  uint8_t tap_y            :  1; /* single-tap on Y axis event */
  uint8_t tap_x            :  1; /* single-tap on X axis event */
  uint8_t tap_sign         :  1; /* sign of tap event (0-pos / 1-neg) */
  uint8_t six_d            :  1; /* orientation change (6D/4D detection) */
  uint8_t six_d_xl         :  1; /* X-axis low 6D/4D event (under threshold) */
  uint8_t six_d_xh         :  1; /* X-axis high 6D/4D event (over threshold) */
  uint8_t six_d_yl         :  1; /* Y-axis low 6D/4D event (under threshold) */
  uint8_t six_d_yh         :  1; /* Y-axis high 6D/4D event (over threshold) */
  uint8_t six_d_zl         :  1; /* Z-axis low 6D/4D event (under threshold) */
  uint8_t six_d_zh         :  1; /* Z-axis high 6D/4D event (over threshold) */
  uint8_t sleep_change     :  1; /* Act/Inact (or Vice-versa) status changed */
  uint8_t sleep_state      :  1; /* Act/Inact status flag (0-Act / 1-Inact) */
  uint8_t step_detector    :  1; /* Step detected */
  uint8_t tilt             :  1; /* Relative tilt event detected */
  uint8_t sig_mot          :  1; /* "significant motion" event detected */
  uint8_t fsm_lc           :  1; /* fsm long counter timeout interrupt event */
  uint8_t fsm1             :  1; /* fsm 1 interrupt event */
  uint8_t fsm2             :  1; /* fsm 2 interrupt event */
  uint8_t fsm3             :  1; /* fsm 3 interrupt event */
  uint8_t fsm4             :  1; /* fsm 4 interrupt event */
  uint8_t fsm5             :  1; /* fsm 5 interrupt event */
  uint8_t fsm6             :  1; /* fsm 6 interrupt event */
  uint8_t fsm7             :  1; /* fsm 7 interrupt event */
  uint8_t fsm8             :  1; /* fsm 8 interrupt event */
  uint8_t fsm9             :  1; /* fsm 9 interrupt event */
  uint8_t fsm10            :  1; /* fsm 10 interrupt event */
  uint8_t fsm11            :  1; /* fsm 11 interrupt event */
  uint8_t fsm12            :  1; /* fsm 12 interrupt event */
  uint8_t fsm13            :  1; /* fsm 13 interrupt event */
  uint8_t fsm14            :  1; /* fsm 14 interrupt event */
  uint8_t fsm15            :  1; /* fsm 15 interrupt event */
  uint8_t fsm16            :  1; /* fsm 16 interrupt event */
  uint8_t mlc1             :  1; /* mlc 1 interrupt event */
  uint8_t mlc2             :  1; /* mlc 2 interrupt event */
  uint8_t mlc3             :  1; /* mlc 3 interrupt event */
  uint8_t mlc4             :  1; /* mlc 4 interrupt event */
  uint8_t mlc5             :  1; /* mlc 5 interrupt event */
  uint8_t mlc6             :  1; /* mlc 6 interrupt event */
  uint8_t mlc7             :  1; /* mlc 7 interrupt event */
  uint8_t mlc8             :  1; /* mlc 8 interrupt event */
  uint8_t sh_endop         :  1; /* sensor hub end operation */
  uint8_t sh_slave0_nack   :  1; /* Not acknowledge on sensor hub slave 0 */
  uint8_t sh_slave1_nack   :  1; /* Not acknowledge on sensor hub slave 1 */
  uint8_t sh_slave2_nack   :  1; /* Not acknowledge on sensor hub slave 2 */
  uint8_t sh_slave3_nack   :  1; /* Not acknowledge on sensor hub slave 3 */
  uint8_t sh_wr_once       :  1; /* "WRITE_ONCE" end on sensor hub slave 0 */
  uint16_t fifo_diff       : 10; /* Number of unread sensor data in FIFO*/
  uint8_t fifo_ovr_latched :  1; /* Latched FIFO overrun status */
  uint8_t fifo_bdr         :  1; /* FIFO Batch counter threshold reached */
  uint8_t fifo_full        :  1; /* FIFO full */
  uint8_t fifo_ovr         :  1; /* FIFO overrun */
  uint8_t fifo_th          :  1; /* FIFO threshold reached */
} LSM6DSO_All_Sources;

typedef struct LSM6DSO_Dev_Calibration_t {
  uint8_t odr_fine_tune;
} LSM6DSO_Dev_Calibration;

typedef struct LSM6DSO_Mode_t{
  struct {
    struct {
      enum {
        LSM6DSO_XL_UI_OFF       = 0x00, /* in power down */
        LSM6DSO_XL_UI_1Hz6_LP   = 0x1B, /* @1Hz6 (low power) */
        LSM6DSO_XL_UI_1Hz6_ULP  = 0x2B, /* @1Hz6 (ultra low/Gy, OIS imu off) */
        LSM6DSO_XL_UI_12Hz5_HP  = 0x01, /* @12Hz5 (high performance) */
        LSM6DSO_XL_UI_12Hz5_LP  = 0x11, /* @12Hz5 (low power) */
        LSM6DSO_XL_UI_12Hz5_ULP = 0x21, /* @12Hz5 (ultra low/Gy, OIS imu off) */
        LSM6DSO_XL_UI_26Hz_HP   = 0x02, /* @26Hz  (high performance) */
        LSM6DSO_XL_UI_26Hz_LP   = 0x12, /* @26Hz  (low power) */
        LSM6DSO_XL_UI_26Hz_ULP  = 0x22, /* @26Hz  (ultra low/Gy, OIS imu off) */
        LSM6DSO_XL_UI_52Hz_HP   = 0x03, /* @52Hz  (high performance) */
        LSM6DSO_XL_UI_52Hz_LP   = 0x13, /* @52Hz  (low power) */
        LSM6DSO_XL_UI_52Hz_ULP  = 0x23, /* @52Hz  (ultra low/Gy, OIS imu off) */
        LSM6DSO_XL_UI_104Hz_HP  = 0x04, /* @104Hz (high performance) */
        LSM6DSO_XL_UI_104Hz_NM  = 0x14, /* @104Hz (normal mode) */
        LSM6DSO_XL_UI_104Hz_ULP = 0x24, /* @104Hz (ultra low/Gy, OIS imu off) */
        LSM6DSO_XL_UI_208Hz_HP  = 0x05, /* @208Hz (high performance) */
        LSM6DSO_XL_UI_208Hz_NM  = 0x15, /* @208Hz (normal mode) */
        LSM6DSO_XL_UI_208Hz_ULP = 0x25, /* @208Hz (ultra low/Gy, OIS imu off) */
        LSM6DSO_XL_UI_416Hz_HP  = 0x06, /* @416Hz (high performance) */
        LSM6DSO_XL_UI_833Hz_HP  = 0x07, /* @833Hz (high performance) */
        LSM6DSO_XL_UI_1667Hz_HP = 0x08, /* @1kHz66 (high performance) */
        LSM6DSO_XL_UI_3333Hz_HP = 0x09, /* @3kHz33 (high performance) */
        LSM6DSO_XL_UI_6667Hz_HP = 0x0A, /* @6kHz66 (high performance) */
      } odr;
      enum {
        LSM6DSO_XL_UI_2g   = 0,
        LSM6DSO_XL_UI_4g   = 2,
        LSM6DSO_XL_UI_8g   = 3,
        LSM6DSO_XL_UI_16g  = 1, /* OIS full scale is also forced to be 16g */
      } fs;
    } xl;
    struct {
      enum {
        LSM6DSO_GY_UI_OFF       = 0x00, /* gy in power down */
        LSM6DSO_GY_UI_12Hz5_LP  = 0x11, /* gy @12Hz5 (low power) */
        LSM6DSO_GY_UI_12Hz5_HP  = 0x01, /* gy @12Hz5 (high performance) */
        LSM6DSO_GY_UI_26Hz_LP   = 0x12, /* gy @26Hz  (low power) */
        LSM6DSO_GY_UI_26Hz_HP   = 0x02, /* gy @26Hz  (high performance) */
        LSM6DSO_GY_UI_52Hz_LP   = 0x13, /* gy @52Hz  (low power) */
        LSM6DSO_GY_UI_52Hz_HP   = 0x03, /* gy @52Hz  (high performance) */
        LSM6DSO_GY_UI_104Hz_NM  = 0x14, /* gy @104Hz (low power) */
        LSM6DSO_GY_UI_104Hz_HP  = 0x04, /* gy @104Hz (high performance) */
        LSM6DSO_GY_UI_208Hz_NM  = 0x15, /* gy @208Hz (low power) */
        LSM6DSO_GY_UI_208Hz_HP  = 0x05, /* gy @208Hz (high performance) */
        LSM6DSO_GY_UI_416Hz_HP  = 0x06, /* gy @416Hz (high performance) */
        LSM6DSO_GY_UI_833Hz_HP  = 0x07, /* gy @833Hz (high performance) */
        LSM6DSO_GY_UI_1667Hz_HP = 0x08, /* gy @1kHz66 (high performance) */
        LSM6DSO_GY_UI_3333Hz_HP = 0x09, /* gy @3kHz33 (high performance) */
        LSM6DSO_GY_UI_6667Hz_HP = 0x0A, /* gy @6kHz66 (high performance) */
      } odr;
      enum {
        LSM6DSO_GY_UI_250dps   = 0,
        LSM6DSO_GY_UI_125dps   = 1,
        LSM6DSO_GY_UI_500dps   = 2,
        LSM6DSO_GY_UI_1000dps  = 4,
        LSM6DSO_GY_UI_2000dps  = 6,
      } fs;
    }gy;
  } ui;
  struct {
    enum {
      LSM6DSO_OIS_ONLY_AUX    = 0x00, /* Auxiliary SPI full control */
      LSM6DSO_OIS_MIXED       = 0x01, /* Enabling by UI / read-config by AUX */
    } ctrl_md;
    struct {
      enum {
        LSM6DSO_XL_OIS_OFF       = 0x00, /* in power down */
        LSM6DSO_XL_OIS_6667Hz_HP = 0x01, /* @6kHz OIS imu active/NO ULP on UI */
      } odr;
      enum {
        LSM6DSO_XL_OIS_2g   = 0,
        LSM6DSO_XL_OIS_4g   = 2,
        LSM6DSO_XL_OIS_8g   = 3,
        LSM6DSO_XL_OIS_16g  = 1, /* UI full scale is also forced to be 16g */
      } fs;
    } xl;
    struct {
      enum {
        LSM6DSO_GY_OIS_OFF       = 0x00, /* in power down */
        LSM6DSO_GY_OIS_6667Hz_HP = 0x01, /* @6kHz No Ultra Low Power*/
      } odr;
      enum {
        LSM6DSO_GY_OIS_250dps   = 0,
        LSM6DSO_GY_OIS_125dps   = 1,
        LSM6DSO_GY_OIS_500dps   = 2,
        LSM6DSO_GY_OIS_1000dps  = 4,
        LSM6DSO_GY_OIS_2000dps  = 6,
      } fs;
    } gy;
  } ois;
  struct {
    enum {
      LSM6DSO_FSM_DISABLE = 0x00,
      LSM6DSO_FSM_XL      = 0x01,
      LSM6DSO_FSM_GY      = 0x02,
      LSM6DSO_FSM_XL_GY   = 0x03,
    } sens;
    enum {
      LSM6DSO_FSM_12Hz5 = 0x00,
      LSM6DSO_FSM_26Hz  = 0x01,
      LSM6DSO_FSM_52Hz  = 0x02,
      LSM6DSO_FSM_104Hz = 0x03,
    } odr;
  } fsm;
} LSM6DSO_Mode;

typedef struct LSM6DSO_Data_t{
  struct {
    struct {
      float mg[3];
      int16_t raw[3];
    }xl;
    struct {
      float mdps[3];
      int16_t raw[3];
    }gy;
    struct {
      float deg_c;
      int16_t raw;
    }heat;
  } ui;
  struct {
    struct {
      float mg[3];
      int16_t raw[3];
    }xl;
    struct {
      float mdps[3];
      int16_t raw[3];
    }gy;
  } ois;
} LSM6DSO_Data;

typedef struct LSM6DSO_Emb_Sens_t{
  uint8_t sig_mot      : 1; /* significant motion */
  uint8_t tilt         : 1; /* tilt detection  */
  uint8_t step         : 1; /* step counter/detector */
  uint8_t step_adv     : 1; /* step counter advanced mode */
  uint8_t fsm          : 1; /* finite state machine */
  uint8_t fifo_compr   : 1; /* FIFO compression */
} LSM6DSO_Emb_Sens;

#endif //LSM6DSO_REGISTER_DEF_H