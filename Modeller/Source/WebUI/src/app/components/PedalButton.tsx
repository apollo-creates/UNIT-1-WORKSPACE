import { useState } from 'react';

interface PedalButtonProps {
  onClick?: () => void;
}

export function PedalButton({ onClick }: PedalButtonProps) {
  const [isActive, setIsActive] = useState(false);

  const handleClick = () => {
    setIsActive(!isActive);
    onClick?.();
  };

  return (
    <>
      {/* Outer Glow Ring */}
      <div className="absolute left-[169px] size-[70px] top-[510px]" data-name="Pedal Glow Ring">
        <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 70">
          <circle 
            cx="35" 
            cy="35" 
            id="Pedal Glow Ring" 
            r="32.5" 
            stroke={isActive ? "#FFD700" : "var(--stroke-0, #4D4D4D)"} 
            strokeOpacity={isActive ? "1" : "0.98"} 
            strokeWidth="5"
            style={{
              filter: isActive ? 'drop-shadow(0 0 8px rgba(255, 215, 0, 0.8))' : 'none',
              transition: 'all 0.2s ease'
            }}
          />
        </svg>
      </div>
      
      {/* Button Container */}
      <button
        onClick={handleClick}
        className="absolute left-[179px] size-[50px] top-[520px] cursor-pointer transition-transform active:scale-95"
        data-name="Pedal Button"
      >
        <div className="absolute inset-[-4%]">
          <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 54 54">
            <g id="Pedal Glow Ring">
              <circle 
                cx="27" 
                cy="27" 
                fill="url(#paint0_linear_1_103)" 
                r="25"
                style={{
                  filter: isActive ? 'brightness(1.3)' : 'brightness(1)',
                  transition: 'filter 0.2s ease'
                }}
              />
              <circle 
                cx="27" 
                cy="27" 
                r="26" 
                stroke="var(--stroke-0, #6F6F6F)" 
                strokeOpacity="0.98" 
                strokeWidth="2" 
              />
            </g>
            <defs>
              <linearGradient gradientUnits="userSpaceOnUse" id="paint0_linear_1_103" x1="27" x2="27" y1="2" y2="52">
                <stop stopColor="#6C6C6C" />
                <stop offset="1" stopColor="#4B4B4B" />
              </linearGradient>
            </defs>
          </svg>
        </div>
      </button>
    </>
  );
}
