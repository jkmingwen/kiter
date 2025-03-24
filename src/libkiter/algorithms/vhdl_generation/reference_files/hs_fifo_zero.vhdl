library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity hs_fifo_zero is
  generic (
    ram_width : natural
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
end hs_fifo_zero;

architecture rtl of hs_fifo_zero is
begin

  buffer_in_ready_0 <= buffer_out_ready_0 and rst;
  buffer_out_valid_0 <= buffer_in_valid_0 and rst;
  buffer_out_data_0 <= buffer_in_data_0;

end architecture;
