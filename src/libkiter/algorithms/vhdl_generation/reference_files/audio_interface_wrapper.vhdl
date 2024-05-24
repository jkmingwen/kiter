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
    $AUDIO_INTERFACE_WRAPPER_PORTS
  );
end audio_interface_wrapper;

architecture structure of audio_interface_wrapper is
  component $ENTITY_NAME is
    $ENTITY_PORTS
  end component $ENTITY_NAME;

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

  $COMPONENT_DECLARATION

  signal sys_clk_sig : std_logic;
  signal rst_sig : std_logic;
  signal mclk_sig : std_logic;

$I2S_TRANSCEIVER_SIGNALS
$INPUT_INTERFACE_SIGNALS
$OUTPUT_INTERFACE_SIGNALS
$I2STOFPC_SIGNALS
$FPCTOI2S_SIGNALS
$ENTITY_SIGNALS

begin

$AUDIO_INTERFACE_WRAPPER_MAPPING
$I2S_TRANSCEIVER_MAPPING
$ENTITY_MAPPING
$INPUT_INTERFACE_MAPPING
$OUTPUT_INTERFACE_MAPPING
$I2STOFPC_COMPONENTS
$FPC2I2S_COMPONENTS

end structure;
