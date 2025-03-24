library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity hs_fifo_one is
  generic (
    ram_width : natural;
    init  : natural := 0
    );
  port (
    clk : in std_logic;
    rst : in std_logic;

    -- hs input interface
    buffer_in_ready_0 : out std_logic;
    buffer_in_valid_0 : in std_logic;
    buffer_in_data_0 : in std_logic_vector(ram_width - 1 downto 0);

    -- hs output interface
    buffer_out_ready_0 : in std_logic;
    buffer_out_valid_0 : out std_logic;
    buffer_out_data_0 : out std_logic_vector(ram_width - 1 downto 0)
    );
end hs_fifo_one;

architecture rtl of hs_fifo_one is

  -- single value ram
  signal present: std_logic := '0';
  signal present_mask : std_logic_vector(buffer_in_data_0'range) := (others => '0');
  signal ram : std_logic_vector(buffer_in_data_0'range) := (others => '0');

  -- local values for output
  signal buffer_in_ready_local :  std_logic;
  signal buffer_out_valid_local : std_logic;

begin


  -- copy internal signals to output

  buffer_in_ready_0 <= rst and  buffer_in_ready_local;
  buffer_out_valid_0 <= rst and buffer_out_valid_local;

  buffer_in_ready_local <= (not present);
  buffer_out_valid_local <= present or buffer_in_valid_0;
  buffer_out_data_0 <= (ram and present_mask) or (buffer_in_data_0 and not present_mask) ;

  present_mask <= (others => present);


  proc_main : process(rst, clk)
  begin
    if rst = '0' then
      if init = 0 then
        present <= '0';
      else
        present <= '1';
        ram <= (others => '0');
      end if;
    else
      if rising_edge(clk) then


        if buffer_in_ready_local = '1' and buffer_in_valid_0 = '1' and buffer_out_ready_0 = '0' and present = '0' then
          ram <= buffer_in_data_0;
          present <= '1';
        elsif buffer_out_ready_0 = '1' and buffer_out_valid_local = '1' and present = '1' then
          present <= '0';
        end if;


      end if;
    end if;
  end process;


end architecture;
