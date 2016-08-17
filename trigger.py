from argparse import ArgumentParser
from serial import Serial
from time import sleep

parser = ArgumentParser()
parser.add_argument('port')
parser.add_argument(
    '-b', '--baud',
    default=9600, dest='baud', type=int, required=False
)
parser.add_argument(
    '-t', '--time',
    default=60, dest='time', type=int, required=False,
)


if __name__ == '__main__':
    args = parser.parse_args()
    print('Try to establish connection')

    trigger_arduino = Serial(
        port=args.port,
        baudrate=args.baud,
        timeout=5,
    )

    line = ''
    while line != 'ready':
        line = trigger_arduino.readline().decode().strip()
        print(line)

    print('Ready')

    trigger_arduino.write(b'1')
    print(trigger_arduino.readline().decode().strip())
    try:
        sleep(args.time)
    except (KeyboardInterrupt, SystemExit):
        pass
    finally:
        trigger_arduino.write(b'0')
        print(trigger_arduino.readline().decode().strip())
