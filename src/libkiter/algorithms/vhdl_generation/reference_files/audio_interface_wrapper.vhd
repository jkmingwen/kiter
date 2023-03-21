-- Wrapper for PL to interface with data coming in from the ADC
-- and to send data out to the DAC
-- Functionality includes:
-- - Expose data from audio codec
-- - Convert data from audio codec (24-bit two's complement) to 34-bit FloPoCo floating point
-- - Implement handshake protocol to make executions synchronous

library ieee;
use ieee.std_logic_1164.ALL;

entity audio_interface_wrapper is
  port (
    ac_bclk : out std_logic;
    ac_pbdat : out std_logic;
    ac_pblrc : out std_logic;
    ac_recdat : in std_logic;
    ac_reclrc : out std_logic;
    sys_clk : in std_logic;
    reset : in std_logic;
    mclk : in std_logic
  );
end audio_interface_wrapper;

architecture structure of audio_interface_wrapper is
  component $ENTITY_NAME is
    $ENTITY_PORTS
  end component $ENTITY_NAME;

  component input_interface is
  port (
    clk : in std_logic;
    ws : in std_logic;
    rst : in std_logic;
    l_valid : out std_logic;
    r_valid : out std_logic;
    l_ready : in std_logic;
    r_ready : in std_logic;
    l_data_in : in std_logic_vector ( 23 downto 0 );
    r_data_in : in std_logic_vector ( 23 downto 0 );
    l_data_out : out std_logic_vector ( 23 downto 0 );
    r_data_out : out std_logic_vector ( 23 downto 0 )
  );
  end component input_interface;

  component i2s_transceiver is
  port (
    reset_n : in std_logic;
    mclk : in std_logic;
    sclk : out std_logic;
    ws : out std_logic;
    sd_tx : out std_logic;
    sd_rx : in std_logic;
    l_data_tx : in std_logic_vector ( 23 downto 0 );
    r_data_tx : in std_logic_vector ( 23 downto 0 );
    l_data_rx : out std_logic_vector ( 23 downto 0 );
    r_data_rx : out std_logic_vector ( 23 downto 0 )
  );
  end component i2s_transceiver;

  component output_interface is
  port (
    clk : in std_logic;
    ws : in std_logic;
    rst : in std_logic;
    l_valid : in std_logic;
    r_valid : in std_logic;
    l_ready : out std_logic;
    r_ready : out std_logic;
    l_data_in : in std_logic_vector ( 23 downto 0 );
    r_data_in : in std_logic_vector ( 23 downto 0 );
    l_data_out : out std_logic_vector ( 23 downto 0 );
    r_data_out : out std_logic_vector ( 23 downto 0 )
  );
  end component output_interface;

  component fpc_to_i2s is
  port (
    clk : in std_logic;
    rst : in std_logic;
    op_in_ready_0 : out std_logic;
    op_in_valid_0 : in std_logic;
    op_in_data_0 : in std_logic_vector ( 33 downto 0 );
    op_out_ready_0 : in std_logic;
    op_out_valid_0 : out std_logic;
    op_out_data_0 : out std_logic_vector ( 23 downto 0 )
  );
  end component fpc_to_i2s;

  component i2s_to_fpc is
  port (
    clk : in std_logic;
    rst : in std_logic;
    op_in_ready_0 : out std_logic;
    op_in_valid_0 : in std_logic;
    op_in_data_0 : in std_logic_vector ( 23 downto 0 );
    op_out_ready_0 : in std_logic;
    op_out_valid_0 : out std_logic;
    op_out_data_0 : out std_logic_vector ( 33 downto 0 )
  );
  end component i2s_to_fpc;

  signal ac_recdat_1 : std_logic;
  signal fpc_to_i2s_0_op_out_data_0 : std_logic_vector ( 23 downto 0 );
  signal fpc_to_i2s_0_op_out_valid_0 : std_logic;
  signal fpc_to_i2s_1_op_out_data_0 : std_logic_vector ( 23 downto 0 );
  signal fpc_to_i2s_1_op_out_valid_0 : std_logic;
  signal i2s_to_fpc_0_op_in_ready_0 : std_logic;
  signal i2s_to_fpc_1_op_in_ready_0 : std_logic;
  signal i2s_transceiver_0_l_data_rx : std_logic_vector ( 23 downto 0 );
  signal i2s_transceiver_0_r_data_rx : std_logic_vector ( 23 downto 0 );
  signal i2s_transceiver_0_sclk : std_logic;
  signal i2s_transceiver_0_sd_tx : std_logic;
  signal i2s_transceiver_0_ws : std_logic;
  signal input_interface_0_l_data_out : std_logic_vector ( 23 downto 0 );
  signal input_interface_0_l_valid : std_logic;
  signal input_interface_0_r_data_out : std_logic_vector ( 23 downto 0 );
  signal input_interface_0_r_valid : std_logic;
  signal output_interface_0_l_data_out : std_logic_vector ( 23 downto 0 );
  signal output_interface_0_l_ready : std_logic;
  signal output_interface_0_r_data_out : std_logic_vector ( 23 downto 0 );
  signal output_interface_0_r_ready : std_logic;
  signal sys_clk_1 : std_logic;
  signal rst_1 : std_logic;
  signal mclk_1 : std_logic;
  signal fpc_to_i2s_0_op_in_ready_0 : std_logic;
  signal fpc_to_i2s_1_op_in_ready_0 : std_logic;
  signal i2s_to_fpc_0_op_out_valid_0 : std_logic;
  signal i2s_to_fpc_0_op_out_data_0 : std_logic_vector ( 33 downto 0 );
  signal i2s_to_fpc_1_op_out_valid_0 : std_logic;
  signal i2s_to_fpc_1_op_out_data_0 : std_logic_vector ( 33 downto 0 );
  signal $ENTITY_NAME_l_out_valid : std_logic;
  signal $ENTITY_NAME_r_out_valid : std_logic;
  signal $ENTITY_NAME_l_out_data : std_logic_vector ( 33 downto 0 );
  signal $ENTITY_NAME_r_out_data : std_logic_vector ( 33 downto 0 );
  signal $ENTITY_NAME_l_in_ready : std_logic;
  signal $ENTITY_NAME_r_in_ready : std_logic;

begin
  ac_bclk <= i2s_transceiver_0_sclk;
  ac_pbdat <= i2s_transceiver_0_sd_tx;
  ac_pblrc <= i2s_transceiver_0_ws;
  ac_recdat_1 <= ac_recdat;
  ac_reclrc <= i2s_transceiver_0_ws;
  sys_clk_1 <= sys_clk;
  mclk_1 <= mclk;
  rst_1 <= reset;

$ENTITY_COMPONENT_MAPPING_MONO

$ENTITY_COMPONENT_MAPPING_STEREO

input_interface_0: component input_interface
     port map (
      clk => sys_clk_1,
      l_data_in(23 downto 0) => i2s_transceiver_0_l_data_rx(23 downto 0),
      l_data_out(23 downto 0) => input_interface_0_l_data_out(23 downto 0),
      l_ready => i2s_to_fpc_1_op_in_ready_0,
      l_valid => input_interface_0_l_valid,
      r_data_in(23 downto 0) => i2s_transceiver_0_r_data_rx(23 downto 0),
      r_data_out(23 downto 0) => input_interface_0_r_data_out(23 downto 0),
      r_ready => i2s_to_fpc_0_op_in_ready_0,
      r_valid => input_interface_0_r_valid,
      rst => rst_1,
      ws => i2s_transceiver_0_ws
    );

output_interface_0: component output_interface
     port map (
      clk => sys_clk_1,
      l_data_in(23 downto 0) => fpc_to_i2s_1_op_out_data_0(23 downto 0),
      l_data_out(23 downto 0) => output_interface_0_l_data_out(23 downto 0),
      l_ready => output_interface_0_l_ready,
      l_valid => fpc_to_i2s_1_op_out_valid_0,
      r_data_in(23 downto 0) => fpc_to_i2s_0_op_out_data_0(23 downto 0),
      r_data_out(23 downto 0) => output_interface_0_r_data_out(23 downto 0),
      r_ready => output_interface_0_r_ready,
      r_valid => fpc_to_i2s_0_op_out_valid_0,
      rst => rst_1,
      ws => i2s_transceiver_0_ws
    );

i2s_to_fpc_0: component i2s_to_fpc
     port map (
      clk => sys_clk_1,
      op_in_data_0(23 downto 0) => input_interface_0_l_data_out(23 downto 0),
      op_in_ready_0 => i2s_to_fpc_0_op_in_ready_0,
      op_in_valid_0 => input_interface_0_l_valid,
      op_out_data_0(33 downto 0) => i2s_to_fpc_0_op_out_data_0(33 downto 0),
      op_out_ready_0 => $ENTITY_NAME_l_in_ready,
      op_out_valid_0 => i2s_to_fpc_0_op_out_valid_0,
      rst => rst_1
    );

i2s_to_fpc_1: component i2s_to_fpc
     port map (
      clk => sys_clk_1,
      op_in_data_0(23 downto 0) => input_interface_0_r_data_out(23 downto 0),
      op_in_ready_0 => i2s_to_fpc_1_op_in_ready_0,
      op_in_valid_0 => input_interface_0_r_valid,
      op_out_data_0(33 downto 0) => i2s_to_fpc_1_op_out_data_0(33 downto 0),
      op_out_ready_0 => $ENTITY_NAME_r_in_ready,
      op_out_valid_0 => i2s_to_fpc_1_op_out_valid_0,
      rst => rst_1
    );

fpc_to_i2s_0: component fpc_to_i2s
     port map (
      clk => sys_clk_1,
      op_in_data_0(33 downto 0) => $ENTITY_NAME_l_out_data,
      op_in_ready_0 => fpc_to_i2s_0_op_in_ready_0,
      op_in_valid_0 => $ENTITY_NAME_l_out_valid,
      op_out_data_0(23 downto 0) => fpc_to_i2s_0_op_out_data_0(23 downto 0),
      op_out_ready_0 => output_interface_0_l_ready,
      op_out_valid_0 => fpc_to_i2s_0_op_out_valid_0,
      rst => rst_1
    );

fpc_to_i2s_1: component fpc_to_i2s
     port map (
      clk => sys_clk_1,
      op_in_data_0(33 downto 0) => $ENTITY_NAME_r_out_data,
      op_in_ready_0 => fpc_to_i2s_1_op_in_ready_0,
      op_in_valid_0 => $ENTITY_NAME_r_out_valid,
      op_out_data_0(23 downto 0) => fpc_to_i2s_1_op_out_data_0(23 downto 0),
      op_out_ready_0 => output_interface_0_r_ready,
      op_out_valid_0 => fpc_to_i2s_1_op_out_valid_0,
      rst => rst_1
    );

i2s_transceiver_0: component i2s_transceiver
     port map (
      l_data_rx(23 downto 0) => i2s_transceiver_0_l_data_rx(23 downto 0),
      l_data_tx(23 downto 0) => output_interface_0_l_data_out(23 downto 0),
      mclk => mclk_1,
      r_data_rx(23 downto 0) => i2s_transceiver_0_r_data_rx(23 downto 0),
      r_data_tx(23 downto 0) => output_interface_0_r_data_out(23 downto 0),
      reset_n => rst_1,
      sclk => i2s_transceiver_0_sclk,
      sd_rx => ac_recdat_1,
      sd_tx => i2s_transceiver_0_sd_tx,
      ws => i2s_transceiver_0_ws
    );

end STRUCTURE;
