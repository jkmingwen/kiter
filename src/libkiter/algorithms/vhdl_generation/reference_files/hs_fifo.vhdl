-- I did some research and found this tutorial looking very simular to Uros buffer
--  https://vhdlwhiz.com/axi-fifo/

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;




entity hs_fifo is
  generic (
    ram_width : natural;
    buffer_size : natural;
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
end hs_fifo;

architecture simulation of hs_fifo is


begin

  assert (buffer_size >= 0) report "Buffer size must be positive or zero" severity error;
  assert (init >= 0) report "Initial marking must be positive or zero" severity error;
  assert (buffer_size >= init) report "Initial marking of buffer overflow" severity error;

  gen_fifo_zero: if (buffer_size = 0) generate

    -- instance
    DUT : entity work.hs_fifo_zero
      generic map ( ram_width => ram_width
                    )
      port map ( clk        => clk,
                 rst      => rst,
                 buffer_in_ready_0 => buffer_in_ready_0,
                 buffer_in_valid_0 => buffer_in_valid_0,
                 buffer_in_data_0  => buffer_in_data_0,
                 buffer_out_ready_0  => buffer_out_ready_0,
                 buffer_out_valid_0  => buffer_out_valid_0,
                 buffer_out_data_0 => buffer_out_data_0);

  end generate gen_fifo_zero;


  gen_fifo_one: if (buffer_size = 1) generate

    -- instance
    DUT : entity work.hs_fifo_one
      generic map ( ram_width => ram_width,
                    init => init
                    )
      port map ( clk        => clk,
                 rst      => rst,
                 buffer_in_ready_0 => buffer_in_ready_0,
                 buffer_in_valid_0 => buffer_in_valid_0,
                 buffer_in_data_0  => buffer_in_data_0,
                 buffer_out_ready_0  => buffer_out_ready_0,
                 buffer_out_valid_0  => buffer_out_valid_0,
                 buffer_out_data_0 => buffer_out_data_0);

  end generate gen_fifo_one;


  gen_fifo_n: if (buffer_size > 1) generate

    -- instance
    DUT : entity work.hs_fifo_n
      generic map ( ram_width => ram_width,
                    buffer_size => buffer_size,
                    init => init
                    )
      port map ( clk        => clk,
                 rst      => rst,
                 buffer_in_ready_0 => buffer_in_ready_0,
                 buffer_in_valid_0 => buffer_in_valid_0,
                 buffer_in_data_0  => buffer_in_data_0,
                 buffer_out_ready_0  => buffer_out_ready_0,
                 buffer_out_valid_0  => buffer_out_valid_0,
                 buffer_out_data_0 => buffer_out_data_0);

  end generate gen_fifo_n;


end architecture;
