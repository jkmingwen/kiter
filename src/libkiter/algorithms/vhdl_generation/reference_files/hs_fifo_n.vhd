library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;



entity hs_fifo_n is
  generic (
    ram_width : natural;
    ram_init  : natural := 0;
    ram_depth : natural := 1
    );
  port (
    buffer_clk : in std_logic;
    buffer_rst : in std_logic;

    -- hs input interface
    buffer_in_ready : out std_logic;
    buffer_in_valid : in std_logic;
    buffer_in_data : in std_logic_vector(ram_width - 1 downto 0);

    -- hs output interface
    buffer_out_ready : in std_logic;
    buffer_out_valid : out std_logic;
    buffer_out_data : out std_logic_vector(ram_width - 1 downto 0)
    );
end hs_fifo_n;

architecture rtl of hs_fifo_n is
    -- This is the RAM of the buffer.
    type ram_type is array (0 to ram_depth - 1) of std_logic_vector (ram_width - 1 downto 0);
    signal ram : ram_type;
    attribute ram_style : string;
    attribute ram_style of ram : signal is "block";
    -- index for RAM
    signal read_index, write_index: natural range 0 to ram_depth - 1 := 0;
    signal fifo_empty, fifo_full: std_logic := '1';
function min_value(A_vector: std_logic_vector; B_natural: natural) return natural is
    variable A_natural : natural;
  begin
    -- Convert A from std_logic_vector to natural
    A_natural := to_integer(unsigned(A_vector));

    -- Compare A and B and return the smaller
    if A_natural < B_natural then
      return A_natural;
    else
      return B_natural;
    end if;
  end function min_value;

begin


  -- FIFO read logic
  read_logic: process(buffer_clk, buffer_rst)
  variable tmp_write_index : natural;
  variable tmp_read_index : natural;
  begin
    if buffer_rst = '0' then

      read_index <= 0;
      write_index <= ram_init;

      if ram_init + 1 = ram_depth then
        fifo_full <= '1';
      else
        fifo_full <= '0';
      end if;

      if ram_init = 0 then
        fifo_empty <= '1';
      else
        fifo_empty <= '0';
      end if;

      --Please don't reset the BRAM that way, or it won't use the BRAM.
      --for i in 0 to ram_depth - 1 loop
      --      ram(i) <= (others => '0');
      --end loop;

    elsif falling_edge(buffer_clk) then

      -- Input behavior
      if buffer_in_valid = '1' and fifo_full = '0' then
        ram(write_index) <= buffer_in_data;
        write_index <= (write_index + 1) mod ram_depth;
        tmp_write_index := (write_index + 1) mod ram_depth;
        else
        tmp_write_index := write_index mod ram_depth;
      end if;

      -- Output behavior
      if buffer_out_ready = '1' and fifo_empty = '0' then
        read_index <= (read_index + 1) mod ram_depth;
        tmp_read_index := (read_index + 1) mod ram_depth;
        else
        tmp_read_index := read_index mod ram_depth;
      end if;

      if (tmp_read_index = tmp_write_index) then
        fifo_empty <= '1' ;
       else
        fifo_empty <= '0';
      end if;

      if (((tmp_write_index + 1) mod ram_depth) = tmp_read_index ) then
        fifo_full <= '1' ;
       else
        fifo_full <= '0';
      end if;


    end if;
  end process read_logic;

  write_logic: process(buffer_clk)
  begin
    if rising_edge(buffer_clk) then
      buffer_in_ready <= not fifo_full;
      buffer_out_valid <= not fifo_empty;
      buffer_out_data <= ram(read_index);
    end if;
  end process write_logic;

end architecture;
