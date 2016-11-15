
################################################################
# This is a generated script based on design: system
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2015.4
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   puts "ERROR: This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source system_script.tcl

# If you do not already have a project created,
# you can create a project using the following command:
#    create_project project_1 myproj -part xc7z020clg484-1
#    set_property BOARD_PART em.avnet.com:zed:part0:1.1 [current_project]

# CHECKING IF PROJECT EXISTS
if { [get_projects -quiet] eq "" } {
   puts "ERROR: Please open or create a project!"
   return 1
}



# CHANGE DESIGN NAME HERE
set design_name system

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "ERROR: Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      puts "INFO: Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   puts "INFO: Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   puts "INFO: Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   puts "INFO: Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

puts "INFO: Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   puts $errMsg
   return $nRet
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set DDR [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR ]
  set FIXED_IO [ create_bd_intf_port -mode Master -vlnv xilinx.com:display_processing_system7:fixedio_rtl:1.0 FIXED_IO ]
  set IIC_FMC [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:iic_rtl:1.0 IIC_FMC ]

  # Create ports
  set GPIO_I [ create_bd_port -dir I -from 31 -to 0 GPIO_I ]
  set GPIO_O [ create_bd_port -dir O -from 31 -to 0 GPIO_O ]
  set GPIO_T [ create_bd_port -dir O -from 31 -to 0 GPIO_T ]
  set hdmi_data [ create_bd_port -dir O -from 15 -to 0 hdmi_data ]
  set hdmi_data_e [ create_bd_port -dir O hdmi_data_e ]
  set hdmi_hsync [ create_bd_port -dir O hdmi_hsync ]
  set hdmi_out_clk [ create_bd_port -dir O hdmi_out_clk ]
  set hdmi_vsync [ create_bd_port -dir O hdmi_vsync ]
  set i2s_bclk [ create_bd_port -dir O -from 0 -to 0 i2s_bclk ]
  set i2s_lrclk [ create_bd_port -dir O -from 0 -to 0 i2s_lrclk ]
  set i2s_mclk [ create_bd_port -dir O -type clk i2s_mclk ]
  set i2s_sdata_in [ create_bd_port -dir I i2s_sdata_in ]
  set i2s_sdata_out [ create_bd_port -dir O -from 0 -to 0 i2s_sdata_out ]
  set iic_mux_scl_I [ create_bd_port -dir I -from 1 -to 0 iic_mux_scl_I ]
  set iic_mux_scl_O [ create_bd_port -dir O -from 1 -to 0 iic_mux_scl_O ]
  set iic_mux_scl_T [ create_bd_port -dir O iic_mux_scl_T ]
  set iic_mux_sda_I [ create_bd_port -dir I -from 1 -to 0 iic_mux_sda_I ]
  set iic_mux_sda_O [ create_bd_port -dir O -from 1 -to 0 iic_mux_sda_O ]
  set iic_mux_sda_T [ create_bd_port -dir O iic_mux_sda_T ]
  set otg_vbusoc [ create_bd_port -dir I otg_vbusoc ]
  set ps_intr_2 [ create_bd_port -dir I ps_intr_2 ]
  set ps_intr_3 [ create_bd_port -dir I ps_intr_3 ]
  set ps_intr_4 [ create_bd_port -dir I ps_intr_4 ]
  set ps_intr_5 [ create_bd_port -dir I ps_intr_5 ]
  set ps_intr_6 [ create_bd_port -dir I ps_intr_6 ]
  set ps_intr_7 [ create_bd_port -dir I ps_intr_7 ]
  set ps_intr_8 [ create_bd_port -dir I ps_intr_8 ]
  set ps_intr_9 [ create_bd_port -dir I ps_intr_9 ]
  set ps_intr_10 [ create_bd_port -dir I ps_intr_10 ]
  set ps_intr_12 [ create_bd_port -dir I ps_intr_12 ]
  set ps_intr_13 [ create_bd_port -dir I ps_intr_13 ]
  set spdif [ create_bd_port -dir O spdif ]

  # Create instance: axi_cpu_interconnect, and set properties
  set axi_cpu_interconnect [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_cpu_interconnect ]
  set_property -dict [ list \
CONFIG.NUM_MI {9} \
CONFIG.STRATEGY {1} \
 ] $axi_cpu_interconnect

  # Create instance: axi_hdmi_clkgen, and set properties
  set axi_hdmi_clkgen [ create_bd_cell -type ip -vlnv analog.com:user:axi_clkgen:1.0 axi_hdmi_clkgen ]

  # Create instance: axi_hdmi_core, and set properties
  set axi_hdmi_core [ create_bd_cell -type ip -vlnv analog.com:user:axi_hdmi_tx:1.0 axi_hdmi_core ]

  # Create instance: axi_hdmi_dma, and set properties
  set axi_hdmi_dma [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.2 axi_hdmi_dma ]
  set_property -dict [ list \
CONFIG.c_include_s2mm {0} \
CONFIG.c_m_axis_mm2s_tdata_width {64} \
CONFIG.c_use_mm2s_fsync {1} \
 ] $axi_hdmi_dma

  # Create instance: axi_hdmi_interconnect, and set properties
  set axi_hdmi_interconnect [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_hdmi_interconnect ]
  set_property -dict [ list \
CONFIG.NUM_MI {1} \
 ] $axi_hdmi_interconnect

  # Create instance: axi_i2s_adi, and set properties
  set axi_i2s_adi [ create_bd_cell -type ip -vlnv analog.com:user:axi_i2s_adi:1.0 axi_i2s_adi ]
  set_property -dict [ list \
CONFIG.C_DMA_TYPE {1} \
CONFIG.C_S_AXI_ADDR_WIDTH {16} \
 ] $axi_i2s_adi

  # Create instance: axi_iic_fmc, and set properties
  set axi_iic_fmc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_iic:2.0 axi_iic_fmc ]

  # Create instance: axi_iic_main, and set properties
  set axi_iic_main [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_iic:2.0 axi_iic_main ]
  set_property -dict [ list \
CONFIG.IIC_BOARD_INTERFACE {Custom} \
CONFIG.USE_BOARD_FLOW {true} \
 ] $axi_iic_main

  # Create instance: axi_spdif_tx_core, and set properties
  set axi_spdif_tx_core [ create_bd_cell -type ip -vlnv analog.com:user:axi_spdif_tx:1.0 axi_spdif_tx_core ]
  set_property -dict [ list \
CONFIG.C_DMA_TYPE {1} \
CONFIG.C_S_AXI_ADDR_WIDTH {16} \
 ] $axi_spdif_tx_core

  # Create instance: axi_timer_0, and set properties
  set axi_timer_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:2.0 axi_timer_0 ]

  # Create instance: axi_timer_1, and set properties
  set axi_timer_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:2.0 axi_timer_1 ]

  # Create instance: sys_audio_clkgen, and set properties
  set sys_audio_clkgen [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:5.2 sys_audio_clkgen ]
  set_property -dict [ list \
CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {12.288} \
CONFIG.PRIM_IN_FREQ {200.000} \
CONFIG.RESET_TYPE {ACTIVE_LOW} \
CONFIG.USE_LOCKED {false} \
CONFIG.USE_RESET {true} \
 ] $sys_audio_clkgen

  # Create instance: sys_concat_intc, and set properties
  set sys_concat_intc [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 sys_concat_intc ]
  set_property -dict [ list \
CONFIG.NUM_PORTS {16} \
 ] $sys_concat_intc

  # Create instance: sys_i2c_mixer, and set properties
  set sys_i2c_mixer [ create_bd_cell -type ip -vlnv analog.com:user:util_i2c_mixer:1.0 sys_i2c_mixer ]

  # Create instance: sys_logic_inv, and set properties
  set sys_logic_inv [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 sys_logic_inv ]
  set_property -dict [ list \
CONFIG.C_OPERATION {not} \
CONFIG.C_SIZE {1} \
 ] $sys_logic_inv

  # Create instance: sys_ps7, and set properties
  set sys_ps7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 sys_ps7 ]
  set_property -dict [ list \
CONFIG.PCW_EN_CLK1_PORT {1} \
CONFIG.PCW_EN_RST1_PORT {1} \
CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {100.0} \
CONFIG.PCW_FPGA1_PERIPHERAL_FREQMHZ {200.0} \
CONFIG.PCW_GPIO_EMIO_GPIO_ENABLE {1} \
CONFIG.PCW_GPIO_EMIO_GPIO_IO {32} \
CONFIG.PCW_IMPORT_BOARD_PRESET {ZedBoard} \
CONFIG.PCW_IRQ_F2P_INTR {1} \
CONFIG.PCW_IRQ_F2P_MODE {REVERSE} \
CONFIG.PCW_TTC0_PERIPHERAL_ENABLE {0} \
CONFIG.PCW_USE_DMA0 {1} \
CONFIG.PCW_USE_DMA1 {1} \
CONFIG.PCW_USE_DMA2 {1} \
CONFIG.PCW_USE_FABRIC_INTERRUPT {1} \
CONFIG.PCW_USE_S_AXI_HP0 {1} \
 ] $sys_ps7

  # Create instance: sys_rstgen, and set properties
  set sys_rstgen [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 sys_rstgen ]
  set_property -dict [ list \
CONFIG.C_EXT_RST_WIDTH {1} \
 ] $sys_rstgen

  # Create interface connections
  connect_bd_intf_net -intf_net axi_cpu_interconnect_M07_AXI [get_bd_intf_pins axi_cpu_interconnect/M07_AXI] [get_bd_intf_pins axi_timer_0/S_AXI]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_M08_AXI [get_bd_intf_pins axi_cpu_interconnect/M08_AXI] [get_bd_intf_pins axi_timer_1/S_AXI]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_m00_axi [get_bd_intf_pins axi_cpu_interconnect/M00_AXI] [get_bd_intf_pins axi_iic_main/S_AXI]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_m01_axi [get_bd_intf_pins axi_cpu_interconnect/M01_AXI] [get_bd_intf_pins axi_hdmi_clkgen/s_axi]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_m02_axi [get_bd_intf_pins axi_cpu_interconnect/M02_AXI] [get_bd_intf_pins axi_hdmi_core/s_axi]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_m03_axi [get_bd_intf_pins axi_cpu_interconnect/M03_AXI] [get_bd_intf_pins axi_hdmi_dma/S_AXI_LITE]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_m04_axi [get_bd_intf_pins axi_cpu_interconnect/M04_AXI] [get_bd_intf_pins axi_spdif_tx_core/S_AXI]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_m05_axi [get_bd_intf_pins axi_cpu_interconnect/M05_AXI] [get_bd_intf_pins axi_i2s_adi/S_AXI]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_m06_axi [get_bd_intf_pins axi_cpu_interconnect/M06_AXI] [get_bd_intf_pins axi_iic_fmc/S_AXI]
  connect_bd_intf_net -intf_net axi_cpu_interconnect_s00_axi [get_bd_intf_pins axi_cpu_interconnect/S00_AXI] [get_bd_intf_pins sys_ps7/M_AXI_GP0]
  connect_bd_intf_net -intf_net axi_hdmi_interconnect_m00_axi [get_bd_intf_pins axi_hdmi_interconnect/M00_AXI] [get_bd_intf_pins sys_ps7/S_AXI_HP0]
  connect_bd_intf_net -intf_net axi_hdmi_interconnect_s00_axi [get_bd_intf_pins axi_hdmi_dma/M_AXI_MM2S] [get_bd_intf_pins axi_hdmi_interconnect/S00_AXI]
  connect_bd_intf_net -intf_net axi_i2s_adi_dma_ack_rx [get_bd_intf_pins axi_i2s_adi/DMA_ACK_RX] [get_bd_intf_pins sys_ps7/DMA2_ACK]
  connect_bd_intf_net -intf_net axi_i2s_adi_dma_ack_tx [get_bd_intf_pins axi_i2s_adi/DMA_ACK_TX] [get_bd_intf_pins sys_ps7/DMA1_ACK]
  connect_bd_intf_net -intf_net axi_i2s_adi_dma_req_rx [get_bd_intf_pins axi_i2s_adi/DMA_REQ_RX] [get_bd_intf_pins sys_ps7/DMA2_REQ]
  connect_bd_intf_net -intf_net axi_i2s_adi_dma_req_tx [get_bd_intf_pins axi_i2s_adi/DMA_REQ_TX] [get_bd_intf_pins sys_ps7/DMA1_REQ]
  connect_bd_intf_net -intf_net axi_iic_fmc_iic [get_bd_intf_ports IIC_FMC] [get_bd_intf_pins axi_iic_fmc/IIC]
  connect_bd_intf_net -intf_net axi_spdif_dma_ack_tx [get_bd_intf_pins axi_spdif_tx_core/DMA_ACK] [get_bd_intf_pins sys_ps7/DMA0_ACK]
  connect_bd_intf_net -intf_net axi_spdif_dma_req_tx [get_bd_intf_pins axi_spdif_tx_core/DMA_REQ] [get_bd_intf_pins sys_ps7/DMA0_REQ]
  connect_bd_intf_net -intf_net sys_ps7_ddr [get_bd_intf_ports DDR] [get_bd_intf_pins sys_ps7/DDR]
  connect_bd_intf_net -intf_net sys_ps7_fixed_io [get_bd_intf_ports FIXED_IO] [get_bd_intf_pins sys_ps7/FIXED_IO]

  # Create port connections
  connect_bd_net -net axi_hdmi_dma_mm2s_introut [get_bd_pins axi_hdmi_dma/mm2s_introut] [get_bd_pins sys_concat_intc/In15]
  connect_bd_net -net axi_hdmi_tx_core_hdmi_clk [get_bd_pins axi_hdmi_clkgen/clk_0] [get_bd_pins axi_hdmi_core/hdmi_clk]
  connect_bd_net -net axi_hdmi_tx_core_hdmi_data [get_bd_ports hdmi_data] [get_bd_pins axi_hdmi_core/hdmi_16_data]
  connect_bd_net -net axi_hdmi_tx_core_hdmi_data_e [get_bd_ports hdmi_data_e] [get_bd_pins axi_hdmi_core/hdmi_16_data_e]
  connect_bd_net -net axi_hdmi_tx_core_hdmi_hsync [get_bd_ports hdmi_hsync] [get_bd_pins axi_hdmi_core/hdmi_16_hsync]
  connect_bd_net -net axi_hdmi_tx_core_hdmi_out_clk [get_bd_ports hdmi_out_clk] [get_bd_pins axi_hdmi_core/hdmi_out_clk]
  connect_bd_net -net axi_hdmi_tx_core_hdmi_vsync [get_bd_ports hdmi_vsync] [get_bd_pins axi_hdmi_core/hdmi_16_vsync]
  connect_bd_net -net axi_hdmi_tx_core_mm2s_fsync [get_bd_pins axi_hdmi_core/m_axis_mm2s_fsync] [get_bd_pins axi_hdmi_core/m_axis_mm2s_fsync_ret] [get_bd_pins axi_hdmi_dma/mm2s_fsync]
  connect_bd_net -net axi_hdmi_tx_core_mm2s_tdata [get_bd_pins axi_hdmi_core/m_axis_mm2s_tdata] [get_bd_pins axi_hdmi_dma/m_axis_mm2s_tdata]
  connect_bd_net -net axi_hdmi_tx_core_mm2s_tkeep [get_bd_pins axi_hdmi_core/m_axis_mm2s_tkeep] [get_bd_pins axi_hdmi_dma/m_axis_mm2s_tkeep]
  connect_bd_net -net axi_hdmi_tx_core_mm2s_tlast [get_bd_pins axi_hdmi_core/m_axis_mm2s_tlast] [get_bd_pins axi_hdmi_dma/m_axis_mm2s_tlast]
  connect_bd_net -net axi_hdmi_tx_core_mm2s_tready [get_bd_pins axi_hdmi_core/m_axis_mm2s_tready] [get_bd_pins axi_hdmi_dma/m_axis_mm2s_tready]
  connect_bd_net -net axi_hdmi_tx_core_mm2s_tvalid [get_bd_pins axi_hdmi_core/m_axis_mm2s_tvalid] [get_bd_pins axi_hdmi_dma/m_axis_mm2s_tvalid]
  connect_bd_net -net axi_iic_fmc_intr [get_bd_pins axi_iic_fmc/iic2intc_irpt] [get_bd_pins sys_concat_intc/In11]
  connect_bd_net -net axi_iic_main_iic2intc_irpt [get_bd_pins axi_iic_main/iic2intc_irpt] [get_bd_pins sys_concat_intc/In14]
  connect_bd_net -net axi_iic_main_scl_i [get_bd_pins axi_iic_main/scl_i] [get_bd_pins sys_i2c_mixer/upstream_scl_O]
  connect_bd_net -net axi_iic_main_scl_o [get_bd_pins axi_iic_main/scl_o] [get_bd_pins sys_i2c_mixer/upstream_scl_I]
  connect_bd_net -net axi_iic_main_scl_t [get_bd_pins axi_iic_main/scl_t] [get_bd_pins sys_i2c_mixer/upstream_scl_T]
  connect_bd_net -net axi_iic_main_sda_i [get_bd_pins axi_iic_main/sda_i] [get_bd_pins sys_i2c_mixer/upstream_sda_O]
  connect_bd_net -net axi_iic_main_sda_o [get_bd_pins axi_iic_main/sda_o] [get_bd_pins sys_i2c_mixer/upstream_sda_I]
  connect_bd_net -net axi_iic_main_sda_t [get_bd_pins axi_iic_main/sda_t] [get_bd_pins sys_i2c_mixer/upstream_sda_T]
  connect_bd_net -net axi_timer_0_interrupt [get_bd_pins axi_timer_0/interrupt] [get_bd_pins sys_concat_intc/In0]
  connect_bd_net -net axi_timer_1_interrupt [get_bd_pins axi_timer_1/interrupt] [get_bd_pins sys_concat_intc/In1]
  connect_bd_net -net i2s_bclk_s [get_bd_ports i2s_bclk] [get_bd_pins axi_i2s_adi/BCLK_O]
  connect_bd_net -net i2s_lrclk_s [get_bd_ports i2s_lrclk] [get_bd_pins axi_i2s_adi/LRCLK_O]
  connect_bd_net -net i2s_sdata_in_s [get_bd_ports i2s_sdata_in] [get_bd_pins axi_i2s_adi/SDATA_I]
  connect_bd_net -net i2s_sdata_out_s [get_bd_ports i2s_sdata_out] [get_bd_pins axi_i2s_adi/SDATA_O]
  connect_bd_net -net ps_intr_10_s [get_bd_ports ps_intr_10] [get_bd_pins sys_concat_intc/In10]
  connect_bd_net -net ps_intr_12_s [get_bd_ports ps_intr_12] [get_bd_pins sys_concat_intc/In12]
  connect_bd_net -net ps_intr_13_s [get_bd_ports ps_intr_13] [get_bd_pins sys_concat_intc/In13]
  connect_bd_net -net ps_intr_2_s [get_bd_ports ps_intr_2] [get_bd_pins sys_concat_intc/In2]
  connect_bd_net -net ps_intr_3_s [get_bd_ports ps_intr_3] [get_bd_pins sys_concat_intc/In3]
  connect_bd_net -net ps_intr_4_s [get_bd_ports ps_intr_4] [get_bd_pins sys_concat_intc/In4]
  connect_bd_net -net ps_intr_5_s [get_bd_ports ps_intr_5] [get_bd_pins sys_concat_intc/In5]
  connect_bd_net -net ps_intr_6_s [get_bd_ports ps_intr_6] [get_bd_pins sys_concat_intc/In6]
  connect_bd_net -net ps_intr_7_s [get_bd_ports ps_intr_7] [get_bd_pins sys_concat_intc/In7]
  connect_bd_net -net ps_intr_8_s [get_bd_ports ps_intr_8] [get_bd_pins sys_concat_intc/In8]
  connect_bd_net -net ps_intr_9_s [get_bd_ports ps_intr_9] [get_bd_pins sys_concat_intc/In9]
  connect_bd_net -net spdif_s [get_bd_ports spdif] [get_bd_pins axi_spdif_tx_core/spdif_tx_o]
  connect_bd_net -net sys_100m_clk [get_bd_pins axi_cpu_interconnect/ACLK] [get_bd_pins axi_cpu_interconnect/M00_ACLK] [get_bd_pins axi_cpu_interconnect/M01_ACLK] [get_bd_pins axi_cpu_interconnect/M02_ACLK] [get_bd_pins axi_cpu_interconnect/M03_ACLK] [get_bd_pins axi_cpu_interconnect/M04_ACLK] [get_bd_pins axi_cpu_interconnect/M05_ACLK] [get_bd_pins axi_cpu_interconnect/M06_ACLK] [get_bd_pins axi_cpu_interconnect/M07_ACLK] [get_bd_pins axi_cpu_interconnect/M08_ACLK] [get_bd_pins axi_cpu_interconnect/S00_ACLK] [get_bd_pins axi_hdmi_clkgen/drp_clk] [get_bd_pins axi_hdmi_clkgen/s_axi_aclk] [get_bd_pins axi_hdmi_core/m_axis_mm2s_clk] [get_bd_pins axi_hdmi_core/s_axi_aclk] [get_bd_pins axi_hdmi_dma/m_axi_mm2s_aclk] [get_bd_pins axi_hdmi_dma/m_axis_mm2s_aclk] [get_bd_pins axi_hdmi_dma/s_axi_lite_aclk] [get_bd_pins axi_hdmi_interconnect/ACLK] [get_bd_pins axi_hdmi_interconnect/M00_ACLK] [get_bd_pins axi_hdmi_interconnect/S00_ACLK] [get_bd_pins axi_i2s_adi/DMA_REQ_RX_ACLK] [get_bd_pins axi_i2s_adi/DMA_REQ_TX_ACLK] [get_bd_pins axi_i2s_adi/S_AXI_ACLK] [get_bd_pins axi_iic_fmc/s_axi_aclk] [get_bd_pins axi_iic_main/s_axi_aclk] [get_bd_pins axi_spdif_tx_core/DMA_REQ_ACLK] [get_bd_pins axi_spdif_tx_core/S_AXI_ACLK] [get_bd_pins axi_timer_0/s_axi_aclk] [get_bd_pins axi_timer_1/s_axi_aclk] [get_bd_pins sys_ps7/DMA0_ACLK] [get_bd_pins sys_ps7/DMA1_ACLK] [get_bd_pins sys_ps7/DMA2_ACLK] [get_bd_pins sys_ps7/FCLK_CLK0] [get_bd_pins sys_ps7/M_AXI_GP0_ACLK] [get_bd_pins sys_ps7/S_AXI_HP0_ACLK] [get_bd_pins sys_rstgen/slowest_sync_clk]
  connect_bd_net -net sys_100m_resetn [get_bd_pins axi_cpu_interconnect/ARESETN] [get_bd_pins axi_cpu_interconnect/M00_ARESETN] [get_bd_pins axi_cpu_interconnect/M01_ARESETN] [get_bd_pins axi_cpu_interconnect/M02_ARESETN] [get_bd_pins axi_cpu_interconnect/M03_ARESETN] [get_bd_pins axi_cpu_interconnect/M04_ARESETN] [get_bd_pins axi_cpu_interconnect/M05_ARESETN] [get_bd_pins axi_cpu_interconnect/M06_ARESETN] [get_bd_pins axi_cpu_interconnect/M07_ARESETN] [get_bd_pins axi_cpu_interconnect/M08_ARESETN] [get_bd_pins axi_cpu_interconnect/S00_ARESETN] [get_bd_pins axi_hdmi_clkgen/s_axi_aresetn] [get_bd_pins axi_hdmi_core/s_axi_aresetn] [get_bd_pins axi_hdmi_dma/axi_resetn] [get_bd_pins axi_hdmi_interconnect/ARESETN] [get_bd_pins axi_hdmi_interconnect/M00_ARESETN] [get_bd_pins axi_hdmi_interconnect/S00_ARESETN] [get_bd_pins axi_i2s_adi/DMA_REQ_RX_RSTN] [get_bd_pins axi_i2s_adi/DMA_REQ_TX_RSTN] [get_bd_pins axi_i2s_adi/S_AXI_ARESETN] [get_bd_pins axi_iic_fmc/s_axi_aresetn] [get_bd_pins axi_iic_main/s_axi_aresetn] [get_bd_pins axi_spdif_tx_core/DMA_REQ_RSTN] [get_bd_pins axi_spdif_tx_core/S_AXI_ARESETN] [get_bd_pins axi_timer_0/s_axi_aresetn] [get_bd_pins axi_timer_1/s_axi_aresetn] [get_bd_pins sys_audio_clkgen/resetn] [get_bd_pins sys_rstgen/peripheral_aresetn]
  connect_bd_net -net sys_200m_clk [get_bd_pins axi_hdmi_clkgen/clk] [get_bd_pins sys_audio_clkgen/clk_in1] [get_bd_pins sys_ps7/FCLK_CLK1]
  connect_bd_net -net sys_200m_resetn [get_bd_pins sys_rstgen/interconnect_aresetn]
  connect_bd_net -net sys_audio_clkgen_clk [get_bd_ports i2s_mclk] [get_bd_pins axi_i2s_adi/DATA_CLK_I] [get_bd_pins axi_spdif_tx_core/spdif_data_clk] [get_bd_pins sys_audio_clkgen/clk_out1]
  connect_bd_net -net sys_aux_reset [get_bd_pins sys_ps7/FCLK_RESET0_N] [get_bd_pins sys_rstgen/ext_reset_in]
  connect_bd_net -net sys_concat_intc_dout [get_bd_pins sys_concat_intc/dout] [get_bd_pins sys_ps7/IRQ_F2P]
  connect_bd_net -net sys_i2c_mixer_downstream_scl_i [get_bd_ports iic_mux_scl_I] [get_bd_pins sys_i2c_mixer/downstream_scl_I]
  connect_bd_net -net sys_i2c_mixer_downstream_scl_o [get_bd_ports iic_mux_scl_O] [get_bd_pins sys_i2c_mixer/downstream_scl_O]
  connect_bd_net -net sys_i2c_mixer_downstream_scl_t [get_bd_ports iic_mux_scl_T] [get_bd_pins sys_i2c_mixer/downstream_scl_T]
  connect_bd_net -net sys_i2c_mixer_downstream_sda_i [get_bd_ports iic_mux_sda_I] [get_bd_pins sys_i2c_mixer/downstream_sda_I]
  connect_bd_net -net sys_i2c_mixer_downstream_sda_o [get_bd_ports iic_mux_sda_O] [get_bd_pins sys_i2c_mixer/downstream_sda_O]
  connect_bd_net -net sys_i2c_mixer_downstream_sda_t [get_bd_ports iic_mux_sda_T] [get_bd_pins sys_i2c_mixer/downstream_sda_T]
  connect_bd_net -net sys_logic_inv_i [get_bd_ports otg_vbusoc] [get_bd_pins sys_logic_inv/Op1]
  connect_bd_net -net sys_logic_inv_o [get_bd_pins sys_logic_inv/Res] [get_bd_pins sys_ps7/USB0_VBUS_PWRFAULT]
  connect_bd_net -net sys_ps7_GPIO_I [get_bd_ports GPIO_I] [get_bd_pins sys_ps7/GPIO_I]
  connect_bd_net -net sys_ps7_GPIO_O [get_bd_ports GPIO_O] [get_bd_pins sys_ps7/GPIO_O]
  connect_bd_net -net sys_ps7_GPIO_T [get_bd_ports GPIO_T] [get_bd_pins sys_ps7/GPIO_T]

  # Create address segments
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces axi_hdmi_dma/Data_MM2S] [get_bd_addr_segs sys_ps7/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_sys_ps7_hp0_ddr_lowocm
  create_bd_addr_seg -range 0x10000 -offset 0x42800000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_timer_0/S_AXI/Reg] SEG_axi_timer_0_Reg
  create_bd_addr_seg -range 0x10000 -offset 0x42810000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_timer_1/S_AXI/Reg] SEG_axi_timer_1_Reg
  create_bd_addr_seg -range 0x10000 -offset 0x79000000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_hdmi_clkgen/s_axi/axi_lite] SEG_data_hdmi_clkgen
  create_bd_addr_seg -range 0x10000 -offset 0x70E00000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_hdmi_core/s_axi/axi_lite] SEG_data_hdmi_core
  create_bd_addr_seg -range 0x10000 -offset 0x43000000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_hdmi_dma/S_AXI_LITE/Reg] SEG_data_hdmi_dma
  create_bd_addr_seg -range 0x10000 -offset 0x77600000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_i2s_adi/S_AXI/reg0] SEG_data_i2s_adi
  create_bd_addr_seg -range 0x10000 -offset 0x41620000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_iic_fmc/S_AXI/Reg] SEG_data_iic_fmc
  create_bd_addr_seg -range 0x10000 -offset 0x41600000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_iic_main/S_AXI/Reg] SEG_data_iic_main
  create_bd_addr_seg -range 0x10000 -offset 0x75C00000 [get_bd_addr_spaces sys_ps7/Data] [get_bd_addr_segs axi_spdif_tx_core/S_AXI/reg0] SEG_data_spdif_core

  # Perform GUI Layout
  regenerate_bd_layout -layout_string {
   guistr: "# # String gsaved with Nlview 6.5.5  2015-06-26 bk=1.3371 VDI=38 GEI=35 GUI=JA:1.6
#  -string -flagsOSRD
preplace port ps_intr_10 -pg 1 -y 1740 -defaultsOSRD
preplace port DDR -pg 1 -y 520 -defaultsOSRD
preplace port ps_intr_12 -pg 1 -y 1780 -defaultsOSRD
preplace port spdif -pg 1 -y 1520 -defaultsOSRD
preplace port hdmi_vsync -pg 1 -y 80 -defaultsOSRD
preplace port hdmi_hsync -pg 1 -y 60 -defaultsOSRD
preplace port ps_intr_13 -pg 1 -y 1800 -defaultsOSRD
preplace port IIC_FMC -pg 1 -y 900 -defaultsOSRD
preplace port ps_intr_2 -pg 1 -y 1580 -defaultsOSRD
preplace port iic_mux_scl_T -pg 1 -y 1400 -defaultsOSRD
preplace port i2s_mclk -pg 1 -y 1080 -defaultsOSRD
preplace port ps_intr_3 -pg 1 -y 1600 -defaultsOSRD
preplace port otg_vbusoc -pg 1 -y 500 -defaultsOSRD
preplace port ps_intr_4 -pg 1 -y 1620 -defaultsOSRD
preplace port i2s_sdata_in -pg 1 -y 550 -defaultsOSRD
preplace port ps_intr_5 -pg 1 -y 1640 -defaultsOSRD
preplace port iic_mux_sda_T -pg 1 -y 1440 -defaultsOSRD
preplace port FIXED_IO -pg 1 -y 540 -defaultsOSRD
preplace port ps_intr_6 -pg 1 -y 1660 -defaultsOSRD
preplace port ps_intr_7 -pg 1 -y 1680 -defaultsOSRD
preplace port hdmi_data_e -pg 1 -y 100 -defaultsOSRD
preplace port hdmi_out_clk -pg 1 -y 40 -defaultsOSRD
preplace port ps_intr_8 -pg 1 -y 1700 -defaultsOSRD
preplace port ps_intr_9 -pg 1 -y 1720 -defaultsOSRD
preplace portBus iic_mux_scl_O -pg 1 -y 1420 -defaultsOSRD
preplace portBus GPIO_T -pg 1 -y 500 -defaultsOSRD
preplace portBus iic_mux_sda_O -pg 1 -y 1460 -defaultsOSRD
preplace portBus GPIO_I -pg 1 -y 450 -defaultsOSRD
preplace portBus hdmi_data -pg 1 -y 120 -defaultsOSRD
preplace portBus iic_mux_scl_I -pg 1 -y 1500 -defaultsOSRD
preplace portBus i2s_sdata_out -pg 1 -y 1500 -defaultsOSRD
preplace portBus iic_mux_sda_I -pg 1 -y 1520 -defaultsOSRD
preplace portBus GPIO_O -pg 1 -y 480 -defaultsOSRD
preplace portBus i2s_lrclk -pg 1 -y 990 -defaultsOSRD
preplace portBus i2s_bclk -pg 1 -y 800 -defaultsOSRD
preplace inst axi_i2s_adi -pg 1 -lvl 4 -y 664 -defaultsOSRD
preplace inst axi_hdmi_clkgen -pg 1 -lvl 3 -y 750 -defaultsOSRD
preplace inst axi_hdmi_core -pg 1 -lvl 5 -y 180 -defaultsOSRD
preplace inst sys_logic_inv -pg 1 -lvl 1 -y 500 -defaultsOSRD
preplace inst axi_iic_main -pg 1 -lvl 3 -y 1380 -defaultsOSRD
preplace inst axi_hdmi_interconnect|s00_couplers|auto_pc -pg 1 -lvl 1 -y 332 -defaultsOSRD
preplace inst sys_concat_intc -pg 1 -lvl 4 -y 1704 -defaultsOSRD
preplace inst axi_hdmi_interconnect|s00_couplers -pg 1 -lvl 1 -y 342 -defaultsOSRD
preplace inst axi_timer_0 -pg 1 -lvl 3 -y 970 -defaultsOSRD
preplace inst sys_audio_clkgen -pg 1 -lvl 5 -y 1140 -defaultsOSRD
preplace inst axi_hdmi_dma -pg 1 -lvl 3 -y 150 -defaultsOSRD
preplace inst sys_i2c_mixer -pg 1 -lvl 5 -y 1410 -defaultsOSRD
preplace inst axi_timer_1 -pg 1 -lvl 3 -y 1170 -defaultsOSRD
preplace inst sys_rstgen -pg 1 -lvl 1 -y 1220 -defaultsOSRD
preplace inst axi_iic_fmc -pg 1 -lvl 5 -y 920 -defaultsOSRD
preplace inst axi_hdmi_interconnect -pg 1 -lvl 4 -y 342 -defaultsOSRD
preplace inst axi_spdif_tx_core -pg 1 -lvl 4 -y 984 -defaultsOSRD
preplace inst axi_cpu_interconnect -pg 1 -lvl 2 -y 840 -defaultsOSRD
preplace inst sys_ps7 -pg 1 -lvl 5 -y 590 -defaultsOSRD
preplace netloc spdif_s 1 4 2 NJ 1000 NJ
preplace netloc sys_logic_inv_o 1 1 5 NJ 500 NJ 850 NJ 864 NJ 850 3130
preplace netloc axi_hdmi_interconnect|axi_hdmi_interconnect_ARESETN_net 1 0 1 1850
preplace netloc sys_i2c_mixer_downstream_sda_t 1 5 1 NJ
preplace netloc ps_intr_2_s 1 0 4 NJ 1580 NJ 1580 NJ 1580 NJ
preplace netloc axi_timer_0_interrupt 1 3 1 1510
preplace netloc sys_ps7_GPIO_O 1 5 1 NJ
preplace netloc ps_intr_10_s 1 0 4 NJ 1740 NJ 1740 NJ 1740 NJ
preplace netloc axi_iic_fmc_intr 1 3 3 1680 1504 NJ 1510 3120
preplace netloc i2s_sdata_in_s 1 0 4 NJ 550 NJ 540 NJ 540 NJ
preplace netloc axi_iic_main_scl_o 1 3 2 NJ 1354 2470
preplace netloc sys_100m_clk 1 0 6 30 1040 370 550 950 290 1650 220 2570 790 3070
preplace netloc sys_ps7_ddr 1 5 1 NJ
preplace netloc sys_ps7_GPIO_T 1 5 1 NJ
preplace netloc axi_hdmi_tx_core_mm2s_tkeep 1 3 2 NJ 130 N
preplace netloc axi_hdmi_interconnect|S00_ARESETN_1 1 0 1 N
preplace netloc sys_100m_resetn 1 1 4 350 530 980 320 1520 484 2580
preplace netloc axi_iic_main_scl_t 1 3 2 NJ 1374 2610
preplace netloc axi_hdmi_tx_core_hdmi_vsync 1 5 1 NJ
preplace netloc axi_i2s_adi_dma_ack_tx 1 3 3 1680 824 NJ 810 3100
preplace netloc ps_intr_5_s 1 0 4 NJ 1640 NJ 1640 NJ 1640 NJ
preplace netloc axi_hdmi_interconnect|S00_ACLK_1 1 0 1 N
preplace netloc axi_cpu_interconnect_m06_axi 1 2 3 NJ 870 NJ 874 2530
preplace netloc sys_200m_resetn 1 1 1 N
preplace netloc sys_ps7_fixed_io 1 5 1 NJ
preplace netloc i2s_bclk_s 1 4 2 NJ 820 NJ
preplace netloc axi_cpu_interconnect_s00_axi 1 1 5 380 10 NJ 10 NJ 10 NJ 380 3110
preplace netloc axi_hdmi_tx_core_hdmi_data 1 5 1 NJ
preplace netloc axi_i2s_adi_dma_ack_rx 1 3 3 1670 814 NJ 800 3090
preplace netloc axi_cpu_interconnect_m04_axi 1 2 2 NJ 860 1620
preplace netloc axi_hdmi_tx_core_mm2s_tlast 1 3 2 NJ 150 N
preplace netloc axi_iic_main_sda_i 1 3 3 NJ 1314 NJ 1300 3080
preplace netloc axi_hdmi_interconnect|axi_hdmi_interconnect_ACLK_net 1 0 1 1840
preplace netloc axi_cpu_interconnect_M08_AXI 1 2 1 900
preplace netloc axi_spdif_dma_ack_tx 1 3 3 1680 844 NJ 830 3120
preplace netloc axi_spdif_dma_req_tx 1 4 1 2510
preplace netloc axi_hdmi_interconnect_s00_axi 1 3 1 1530
preplace netloc ps_intr_4_s 1 0 4 NJ 1620 NJ 1620 NJ 1620 NJ
preplace netloc axi_hdmi_tx_core_hdmi_out_clk 1 5 1 NJ
preplace netloc ps_intr_9_s 1 0 4 NJ 1720 NJ 1720 NJ 1720 NJ
preplace netloc axi_cpu_interconnect_m00_axi 1 2 1 920
preplace netloc axi_cpu_interconnect_m03_axi 1 2 1 900
preplace netloc axi_iic_main_sda_o 1 3 2 NJ 1414 2480
preplace netloc i2s_sdata_out_s 1 4 2 NJ 1520 NJ
preplace netloc axi_hdmi_interconnect|s00_couplers|S_ACLK_1 1 0 1 2000
preplace netloc axi_i2s_adi_dma_req_tx 1 4 1 2470
preplace netloc axi_hdmi_tx_core_mm2s_tdata 1 3 2 NJ 110 N
preplace netloc sys_200m_clk 1 2 4 1010 1070 NJ 1104 2610 1060 3110
preplace netloc axi_cpu_interconnect_m05_axi 1 2 2 960 550 NJ
preplace netloc ps_intr_3_s 1 0 4 NJ 1600 NJ 1600 NJ 1600 NJ
preplace netloc i2s_lrclk_s 1 4 2 NJ 990 NJ
preplace netloc sys_i2c_mixer_downstream_scl_i 1 0 5 NJ 1510 NJ 1510 NJ 1510 NJ 1454 NJ
preplace netloc axi_hdmi_interconnect|s00_couplers_to_axi_hdmi_interconnect 1 1 1 N
preplace netloc sys_concat_intc_dout 1 4 1 2590
preplace netloc ps_intr_13_s 1 0 4 NJ 1800 NJ 1800 NJ 1800 NJ
preplace netloc axi_cpu_interconnect_m01_axi 1 2 1 940
preplace netloc axi_iic_main_iic2intc_irpt 1 3 1 1540
preplace netloc sys_aux_reset 1 0 6 20 560 NJ 560 NJ 840 NJ 854 NJ 840 3080
preplace netloc ps_intr_6_s 1 0 4 NJ 1660 NJ 1660 NJ 1660 NJ
preplace netloc axi_hdmi_interconnect_m00_axi 1 4 1 2590
preplace netloc axi_iic_main_sda_t 1 3 2 NJ 1424 2470
preplace netloc sys_audio_clkgen_clk 1 3 3 1660 1094 NJ 1080 3130
preplace netloc axi_hdmi_tx_core_hdmi_data_e 1 5 1 NJ
preplace netloc sys_i2c_mixer_downstream_sda_i 1 0 5 NJ 1520 NJ 1520 NJ 1520 NJ 1474 NJ
preplace netloc ps_intr_12_s 1 0 4 NJ 1780 NJ 1780 NJ 1780 NJ
preplace netloc axi_hdmi_interconnect|s00_couplers|s00_couplers_to_auto_pc 1 0 1 N
preplace netloc sys_i2c_mixer_downstream_scl_o 1 5 1 NJ
preplace netloc axi_hdmi_interconnect|s00_couplers|auto_pc_to_s00_couplers 1 1 1 N
preplace netloc axi_timer_1_interrupt 1 3 1 1630
preplace netloc axi_i2s_adi_dma_req_rx 1 4 1 2490
preplace netloc axi_cpu_interconnect_m02_axi 1 2 3 930 280 NJ 70 NJ
preplace netloc ps_intr_8_s 1 0 4 NJ 1700 NJ 1700 NJ 1700 NJ
preplace netloc sys_logic_inv_i 1 0 1 NJ
preplace netloc axi_cpu_interconnect_M07_AXI 1 2 1 910
preplace netloc axi_iic_fmc_iic 1 5 1 NJ
preplace netloc sys_i2c_mixer_downstream_sda_o 1 5 1 NJ
preplace netloc axi_hdmi_tx_core_hdmi_hsync 1 5 1 NJ
preplace netloc ps_intr_7_s 1 0 4 NJ 1680 NJ 1680 NJ 1680 NJ
preplace netloc sys_i2c_mixer_downstream_scl_t 1 5 1 NJ
preplace netloc sys_ps7_GPIO_I 1 0 6 NJ 450 NJ 450 NJ 450 NJ 474 NJ 390 3090
preplace netloc axi_iic_main_scl_i 1 3 3 NJ 1324 NJ 1310 3070
preplace netloc axi_hdmi_tx_core_hdmi_clk 1 3 2 1620 210 NJ
preplace netloc axi_hdmi_tx_core_mm2s_tready 1 3 2 NJ 160 2570
preplace netloc axi_hdmi_tx_core_mm2s_tvalid 1 3 2 NJ 170 N
preplace netloc axi_hdmi_tx_core_mm2s_fsync 1 2 4 1010 20 NJ 20 2600 370 3110
preplace netloc axi_hdmi_interconnect|s00_couplers|S_ARESETN_1 1 0 1 2010
preplace netloc axi_hdmi_interconnect|axi_hdmi_interconnect_to_s00_couplers 1 0 1 N
preplace netloc axi_hdmi_dma_mm2s_introut 1 3 1 1600
levelinfo -pg 1 0 190 750 1307 1930 2840 3160 -top 0 -bot 1910
levelinfo -hier axi_hdmi_interconnect|s00_couplers * 2140 *
levelinfo -hier axi_hdmi_interconnect * 2050 *
",
}

  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


