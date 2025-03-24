-- Testbench to check audio in/out from I2S transceiver
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_TEXTIO.all;
use std.textio.all;

entity tb_audio_in_out is
--  Port ( );
end tb_audio_in_out;

architecture Behavioral of tb_audio_in_out is

  signal clk : std_logic := '0';
  signal reset : std_logic := '1';

  -- Signals for I2S interface
  -- signal mclk : std_logic := '0'; -- Master Clock
  signal bclk : std_logic := '0'; -- Bit Clock
  signal pblrclk : std_logic := '0'; -- Left-Right Clock
  signal reclrclk : std_logic := '0'; -- Left-Right Clock
  signal data_in : std_logic := '0';
  signal data_out : std_logic := '0';
  --signal ws : std_logic := '0'; -- Word select line

  -- Clock period
  constant clk_period             : time := 8 ns;

  -- Parameters for the test input
  constant NUMBER_OF_INPUT_WORDS  : integer := 12; -- length of an input vector
  constant NUMBER_OF_OUTPUT_WORDS : integer := 12; -- length of an output vector
  constant NUMBER_OF_TEST_VECTORS : integer := 1; -- number of such test vectors (cases)
  constant width                  : integer := 32; -- width of an input vector

  type test_input_memory_type is array (0 to NUMBER_OF_TEST_VECTORS * NUMBER_OF_INPUT_WORDS - 1) of std_logic_vector(width - 1 downto 0);
  type test_result_memory_type is array (0 to NUMBER_OF_TEST_VECTORS * NUMBER_OF_OUTPUT_WORDS - 1) of std_logic_vector(width - 1 downto 0);

  signal test_input_memory           : test_input_memory_type;
  --signal test_result_expected_memory : test_result_memory_type;
  --signal result_memory               : test_result_memory_type; -- same size as test_result_expected_memory

  -- Procedure for clock generation
  procedure clk_gen(signal clk : out std_logic; constant FREQ : real) is
    constant PERIOD    : time := 1 sec / FREQ;        -- Full period
    constant HIGH_TIME : time := PERIOD / 2;          -- High time
    constant LOW_TIME  : time := PERIOD - HIGH_TIME;  -- Low time; always >= HIGH_TIME
  begin
    -- Check the arguments
    assert (HIGH_TIME /= 0 fs) report "clk_plain: High time is zero; time resolution to large for frequency" severity FAILURE;
    -- Generate a clock cycle
    loop
      clk <= '1';
      wait for HIGH_TIME;
      clk <= '0';
      wait for LOW_TIME;
    end loop;
  end procedure;

  signal clk_8n: std_logic;
  signal clk_80n: std_logic;

begin

  -- Clock Generation Process
  clk_process: process
  begin
    wait for clk_period / 2; -- Half of the desired clock period
    clk <= not clk;
  end process;

  -- Clock generation with concurrent procedure call
  clk_gen(clk_8n, 250.0E6);  -- 250.0 MHz clock
  clk_gen(clk_80n, 12.288E6);  -- 12.288 MHz clock

  -- Reset Generation Process
  reset_process: process
  begin
    reset <= '0';
    wait for 20 ns;
    reset <= '1';
    wait;
  end process;

  -- Instantiate the I2S Transceiver Module
  i2s_transceiver_inst : entity work.audio_interface_wrapper
    port map (
      mclk => clk_80n,
      reset => reset,
      ac_bclk_0 => bclk,
      sys_clk => clk_8n,
      ac_recdat_0 => data_in,
      ac_pbdat_0 => data_out,
      ac_pblrc_0 => pblrclk,
      ac_reclrc_0 => reclrclk
      );

  -- Stimulus Generation Process
  stimulus_process: process
    file test_input           : TEXT open READ_MODE is "audio_test0.mem";
    file test_result_expected : TEXT open READ_MODE is "audio_test0.mem";
    variable Lr               : line;
    variable Lr_hex           : std_logic_vector(width - 1 downto 0);
    variable word_cnt         : integer := 0;
    variable success          : std_logic := '1';
    variable mem_bit          : std_logic;
  begin
    -- Read .mem file into memory
    report ("Loading Memory.");
    -- Loading Input Data
    word_cnt := 0;
    while not endfile(test_input) loop
      readline (test_input, Lr);
      if Lr.all'LENGTH = 0 or (Lr.all(1) = '/' and Lr.all(2) = '/') then -- skipping blank lines and those starting with //
        next;
      else
        hread(Lr, Lr_hex); -- Read the argument as hex value
        test_input_memory(word_cnt) <= Lr_hex;
        word_cnt := word_cnt + 1;
      end if;
    end loop;
    wait for 20 ns; -- Wait before applying stimuli
    -- Wait for 20ns because reset is 20 ns

    -- Simulate data transmission
    report("Starting Data Transmission.");
    word_cnt := 0;
    while word_cnt < NUMBER_OF_INPUT_WORDS loop
      Lr_hex := test_input_memory(word_cnt) ( width-1 downto 0 );
      for i in width-1 downto 0 loop
        wait until rising_edge(bclk);
        data_in <= Lr_hex(i);
      end loop;
      word_cnt := word_cnt + 1;
    end loop;
    report("Finished Data Transmission");
    wait for 1000 ns;

    wait;
  end process;

end Behavioral;

