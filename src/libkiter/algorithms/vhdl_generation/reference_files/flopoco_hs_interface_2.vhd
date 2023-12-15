library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- top-level entity declaration
entity $ENTITY_NAME is
  port ( clk : in std_logic;
         rst : in std_logic;

         op_in_ready_0 : out std_logic;
         op_in_valid_0 : in std_logic;
         op_in_data_0  : in std_logic_vector(33 downto 0) := (others => '0');

         op_in_ready_1 : out std_logic;
         op_in_valid_1 : in std_logic;
         op_in_data_1  : in std_logic_vector(33 downto 0) := (others => '0');

         op_out_ready_0  : in std_logic;
         op_out_valid_0  : out std_logic;
         op_out_data_0   : out std_logic_vector(33 downto 0) := (others => '0') );
end $ENTITY_NAME;

architecture connections of $ENTITY_NAME is
  constant operator_lifespan : integer := $OP_LIFESPAN;
  constant bit_width : integer := 34;
  -- lower-level component declaration;
  component hs_merger$HSM_TYPE is
    generic (bit_width : integer);
    port ( clk        : in std_logic;
           reset      : in std_logic;
           in_ready_0 : out std_logic;
           in_valid_0 : in std_logic;
           in_data_0  : in std_logic_vector (bit_width-1 downto 0);
           in_ready_1 : out std_logic;
           in_valid_1 : in std_logic;
           in_data_1  : in std_logic_vector (bit_width-1 downto 0);
           out_ready  : in std_logic; -- output data is sent together so they share ready and valid signals
           out_valid  : out std_logic;
           out_data_0 : out std_logic_vector (bit_width-1 downto 0);
           out_data_1 : out std_logic_vector (bit_width-1 downto 0) );
  end component;

  component countdown is
    generic ( operator_lifespan : integer := operator_lifespan);
    port ( clk           : in std_logic;
           reset         : in std_logic;
           in_ready      : out std_logic;
           in_valid      : in std_logic;
           can_store     : in std_logic;
           count         : out integer := operator_lifespan; -- just for debugging
           trigger_store : out std_logic := '0' );
  end component;

  component store_send is
    generic ( data_bit_width : integer);
    Port ( clk           : in std_logic;
           reset         : in std_logic;
           trigger_store : in std_logic;
           in_data       : in std_logic_vector (data_bit_width-1 downto 0);
           out_ready     : in std_logic;
           out_valid     : out std_logic;
           out_data      : out std_logic_vector (data_bit_width-1 downto 0);
           can_store     : out std_logic );
  end component;

  component $FLOPOCO_OP_NAME is
    port (clk : in std_logic;
          X   : in  std_logic_vector(8+23+2 downto 0);
          Y   : in  std_logic_vector(8+23+2 downto 0);
          R   : out std_logic_vector(8+23+2 downto 0) );
  end component;

  -- internal signals
  signal cd_in_ready, cd_in_valid, cd_trigger_store,
    ss_can_store : std_logic;
  signal hsm_out_data0, hsm_out_data1,
    flopoco_out_result : std_logic_vector (bit_width-1 downto 0);

begin

  hsm: hs_merger$HSM_TYPE
    generic map ( bit_width => bit_width )
    port map ( clk        => clk,
               reset      => rst,
               in_ready_0 => op_in_ready_0,
               in_valid_0 => op_in_valid_0,
               in_data_0  => op_in_data_0,
               in_ready_1 => op_in_ready_1,
               in_valid_1 => op_in_valid_1,
               in_data_1  => op_in_data_1,
               out_ready  => cd_in_ready,
               out_valid  => cd_in_valid,
               out_data_0 => hsm_out_data0,
               out_data_1 => hsm_out_data1 );

  $COMPONENT_NAME : $FLOPOCO_OP_NAME port map ( clk => clk,
                                                 X => hsm_out_data0,
                                                 Y => hsm_out_data1,
                                                 R => flopoco_out_result );

  cd: countdown port map ( clk           => clk,
                           reset         => rst,
                           in_ready      => cd_in_ready,
                           in_valid      => cd_in_valid,
                           can_store     => ss_can_store,
                           trigger_store => cd_trigger_store );

  ss: store_send
    generic map ( data_bit_width => bit_width )
    port map ( clk           => clk,
               trigger_store => cd_trigger_store,
               out_ready     => op_out_ready_0,
               in_data       => flopoco_out_result,
               out_valid     => op_out_valid_0,
               out_data      => op_out_data_0,
               can_store     => ss_can_store,
               reset         => rst );

end connections;
