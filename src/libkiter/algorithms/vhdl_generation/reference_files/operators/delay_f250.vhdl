library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity delay_f250 is
  generic (
        ram_width : natural;
        init  : natural := 0;
        buffer_size : natural := 1;
        push_start : integer; -- start time of write operation
        pop_start : integer -- start time of read operation
        );
  port ( clk : in std_logic;
         rst : in std_logic;
         cycle_count : in integer;
         op_in_data_0  : in std_logic_vector(33 downto 0) ; -- data in
         op_in_data_1  : in std_logic_vector(33 downto 0) ; -- delay amount
         op_out_data_0   : out std_logic_vector(33 downto 0)
          );
end delay_f250;

architecture connections of delay_f250 is

  -- This is the RAM of the buffer.
  type ram_type is array (0 to buffer_size - 1) of std_logic_vector (ram_width - 1 downto 0);
  signal ram : ram_type;
  attribute ram_style : string;
  attribute ram_style of ram : signal is "block";
  -- index for RAM
  signal read_index, write_index: natural range 0 to buffer_size-1 := 0;
  signal fifo_empty, cmd_ready, fifo_full: std_logic := '1';
  signal last_cmd : std_logic_vector  (0 to ram_width-1);

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
    if rst = '0' then

      read_index <= 0;
      write_index <= init;

      if init + 1 = buffer_size then
        fifo_full <= '1';
      else
        fifo_full <= '0';
      end if;
      if init = 0 then
        fifo_empty <= '1';
      else
        fifo_empty <= '0';
      end if;

    elsif rising_edge(clk) then

      -- Input behavior
      if cycle_count = push_start and fifo_full = '0' then
        ram(write_index) <= op_in_data_0;
        write_index <= (write_index + 1) mod buffer_size;
        tmp_write_index := (write_index + 1) mod buffer_size;
        else
        tmp_write_index := write_index mod buffer_size;
      end if;

      -- Output behavior
      if cycle_count = pop_start and fifo_empty = '0' then
        op_out_data_0 <= ram(read_index);
        read_index <= (read_index + 1) mod buffer_size;
        tmp_read_index := (read_index + 1) mod buffer_size;
        else
        tmp_read_index := read_index mod buffer_size;
      end if;

      -- update buffer size
      if last_cmd /= op_in_data_1 then
        tmp_read_index := 0;
        tmp_write_index := min_value(op_in_data_1, buffer_size);
        write_index <= tmp_write_index;
        read_index <= tmp_read_index;
      end if;

      if (tmp_read_index = tmp_write_index) then
        fifo_empty <= '1' ;
      else
        fifo_empty <= '0' ;
      end if;
      if (((tmp_write_index + 1) mod buffer_size) = tmp_read_index) then
        fifo_full <= '1' ;
      else
        fifo_full <= '0' ;
      end if;

      last_cmd <= op_in_data_1;

    end if;
  end process read_logic;

end connections;


