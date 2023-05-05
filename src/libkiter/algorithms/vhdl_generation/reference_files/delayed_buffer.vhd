----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 05.05.2023 10:21:00
-- Design Name: 
-- Module Name: delayed_buffer - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;


entity delayed_buffer is
generic (
    ram_width : natural;
    ram_init  : natural := 0;
    ram_depth : natural := 1
    );
port (

    buffer_clk : in std_logic;
    buffer_rst : in std_logic;

    buffer_in_ready : out std_logic;
    buffer_in_valid : in std_logic;
    buffer_in_data : in std_logic_vector(ram_width - 1 downto 0);

    cmd_in_ready : out std_logic;
    cmd_in_valid : in std_logic;
    cmd_in_data : in std_logic_vector(34 - 1 downto 0);

    buffer_out_ready : in std_logic;
    buffer_out_valid : out std_logic;
    buffer_out_data : out std_logic_vector(ram_width - 1 downto 0)


); 
end delayed_buffer;

architecture rtl of delayed_buffer is

    -- This is the RAM of the buffer. 
    type ram_type is array (0 to ram_depth - 1) of std_logic_vector (ram_width - 1 downto 0);
    signal ram : ram_type;
    attribute ram_style : string;
    attribute ram_style of ram : signal is "block";
    


  -- newest element at head, oldest element at tail
  subtype index_type is natural range ram_type'range;
  signal head : index_type;
  signal tail : index_type;

  -- local values for outputs
  signal cmd_local :  std_logic_vector(34 - 1 downto 0);
  signal in_ready_local :  std_logic;
  signal out_valid_local : std_logic;
  signal out_data_local :   std_logic_vector(ram_width - 1 downto 0);

  -- increment or wrap the index if this transaction is valid
  function next_index(
    index : index_type;
    ready : std_logic;
    valid : std_logic) return index_type is
  begin
    if ready = '1' and valid = '1' then
      if index = index_type'high then
        return index_type'low;
      else
        return index + 1;
      end if;
    end if;

    return index;
  end function;

-- Define the function
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


  -- Local values affect outputs




  proc_tail_head_count_inready_outvalid : process(buffer_rst, buffer_clk)
    variable next_tail : index_type;
    variable next_head : index_type;
    variable next_count : index_type;
  begin


    if buffer_rst = '0' then
    
      tail <= index_type'low;
      head <= index_type'low + ram_init;
      
      in_ready_local <= '0';
      out_valid_local <= '0';

      buffer_in_ready <=   '0';
      buffer_out_valid <=   '0';

      buffer_out_data <= (others => '0');
      
      ram <= (others => (others => '0'));
      
      cmd_local <= (others => '0');
      cmd_in_ready <= '1'; -- we assume input will be unlimited, and we have unlimited storage.
      
    else

      if falling_edge(buffer_clk) then
      
      
      
        -- This is an addition for variable delay buffer.
      
        if (cmd_in_valid = '1') then -- input data is valid, we are already ready.
        
        if (cmd_local = cmd_in_data) then
            cmd_local <= cmd_in_data;
        else -- The cmd change, we reset the buffer to ensure the delay is correct.
        
            tail <= index_type'low;
            head <= index_type'low + min_value(cmd_local,ram_depth); -- The delay cannot be longer than the buffer size.
            ram <= (others => (others => '0'));
            
            cmd_local <= cmd_in_data;
        end if;
        
        end if;
        
      
      

        next_head := next_index(head, in_ready_local, buffer_in_valid);
        next_tail := next_index(tail, buffer_out_ready, out_valid_local);

        if next_head < next_tail then
          next_count := next_head - next_tail + ram_depth + 1;
        else
          next_count := next_head - next_tail;
        end if;


        -- At this point, next_* is validated, this is going to be the future
        -- Given what we know from the future, we can make decision


        if next_count < ram_depth then
          in_ready_local <= '1';
        else
          in_ready_local <= '0';
        end if;

        if next_count > 0 then
          out_valid_local <= '1';
        else
          out_valid_local <= '0';
        end if;

    
        tail <= next_tail;
        head <= next_head;
        ram(head) <= buffer_in_data;

        -- We will have a delay in the output if we have tail and head at the same location
        if head = next_tail then
          out_data_local <= buffer_in_data;
        else
          out_data_local <= ram(next_tail);
        end if;
      end if;
      
      if rising_edge(buffer_clk) then

        buffer_in_ready <=  in_ready_local;
        buffer_out_valid <=  out_valid_local;
        buffer_out_data <=  out_data_local;

      end if;
      
    end if;
  end process;




end architecture;

