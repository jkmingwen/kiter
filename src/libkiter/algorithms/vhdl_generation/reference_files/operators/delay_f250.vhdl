library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity delay is
generic (
    ram_width : natural;
    ram_init  : natural := 0;
    ram_depth : natural := 1
    );
  port ( clk : in std_logic;
         rst : in std_logic;

         op_in_ready_0 : out std_logic;
         op_in_valid_0 : in std_logic;
         op_in_data_0  : in std_logic_vector(33 downto 0) ;

         op_in_ready_1 : out std_logic;
         op_in_valid_1 : in std_logic;
         op_in_data_1  : in std_logic_vector(33 downto 0) ;

         op_out_ready_0  : in std_logic;
         op_out_valid_0  : out std_logic;
         op_out_data_0   : out std_logic_vector(33 downto 0)
          );
end delay;

architecture connections of delay is

    -- This is the RAM of the buffer.
    type ram_type is array (0 to ram_depth - 1) of std_logic_vector (ram_width - 1 downto 0);
    signal ram : ram_type;
    attribute ram_style : string;
    attribute ram_style of ram : signal is "block";
    -- index for RAM
    signal read_index, write_index: natural range 0 to ram_depth - 1 := 0;
    signal fifo_empty, cmd_ready, fifo_full: std_logic := '1';
    signal last_cmd : std_logic_vector  (0 to ram_width - 1);
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
  read_logic: process(clk, rst)
  variable tmp_write_index : natural;
  variable tmp_read_index : natural;
  begin
    if rst = '1' then

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

      cmd_ready <= '0';

    elsif rising_edge(clk) then

      -- Input behavior
      if op_in_valid_0 = '1' and fifo_full = '0' then
        ram(write_index) <= op_in_data_0;
        write_index <= (write_index + 1) mod ram_depth;
        tmp_write_index := (write_index + 1) mod ram_depth;
        else
        tmp_write_index := write_index mod ram_depth;
      end if;

      -- Output behavior
      if op_out_ready_0 = '1' and fifo_empty = '0' then
        op_out_data_0 <= ram(read_index);
        read_index <= (read_index + 1) mod ram_depth;
        tmp_read_index := (read_index + 1) mod ram_depth;
        else
        tmp_read_index := read_index mod ram_depth;
      end if;

      if op_in_valid_1 = '1' and cmd_ready = '1' and last_cmd /= op_in_data_1 then
        tmp_read_index := 0;
        tmp_write_index := min_value(op_in_data_1, ram_depth);
        write_index <= tmp_write_index;
        read_index <= tmp_read_index;
      end if;
       
      if (tmp_read_index = tmp_write_index) then
      fifo_empty <= '1' ;
      else 
      fifo_empty <= '0' ;
      end if;
      if (((tmp_write_index + 1) mod ram_depth) = tmp_read_index) then
      fifo_full <= '1' ;
      else 
      fifo_full <= '0' ;
      end if;
      
      cmd_ready <= '1';
      last_cmd <= op_in_data_1;

    end if;
  end process read_logic;


  -- Outputs
  op_in_ready_1 <= cmd_ready;
  op_in_ready_0 <= not fifo_full;
  op_out_valid_0 <= not fifo_empty;

end connections;


