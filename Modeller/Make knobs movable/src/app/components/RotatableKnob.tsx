import { useState, useRef, useEffect, useMemo } from 'react';
import svgPaths from "../../imports/svg-jlepp2ol9g";

interface RotatableKnobProps {
  initialRotation?: number;
  onRotationChange?: (rotation: number) => void;
}

export function RotatableKnob({ initialRotation = 0, onRotationChange }: RotatableKnobProps) {
  const [rotation, setRotation] = useState(initialRotation);
  const [isDragging, setIsDragging] = useState(false);
  const knobRef = useRef<HTMLDivElement>(null);
  const lastAngleRef = useRef<number>(0);
  
  // Generate unique IDs for this knob instance
  const uniqueId = useMemo(() => Math.random().toString(36).substring(7), []);

  const handleMouseDown = (e: React.MouseEvent) => {
    e.preventDefault();
    setIsDragging(true);
    
    if (knobRef.current) {
      const rect = knobRef.current.getBoundingClientRect();
      const centerX = rect.left + rect.width / 2;
      const centerY = rect.top + rect.height / 2;
      const angle = Math.atan2(e.clientY - centerY, e.clientX - centerX);
      lastAngleRef.current = angle;
    }
  };

  useEffect(() => {
    const handleMouseMove = (e: MouseEvent) => {
      if (!isDragging || !knobRef.current) return;

      const rect = knobRef.current.getBoundingClientRect();
      const centerX = rect.left + rect.width / 2;
      const centerY = rect.top + rect.height / 2;
      
      const angle = Math.atan2(e.clientY - centerY, e.clientX - centerX);
      const delta = angle - lastAngleRef.current;
      
      // Handle angle wrap-around
      let adjustedDelta = delta;
      if (delta > Math.PI) adjustedDelta -= 2 * Math.PI;
      if (delta < -Math.PI) adjustedDelta += 2 * Math.PI;
      
      const newRotation = rotation + (adjustedDelta * 180 / Math.PI);
      
      setRotation(newRotation);
      onRotationChange?.(newRotation);
      lastAngleRef.current = angle;
    };

    const handleMouseUp = () => {
      setIsDragging(false);
    };

    if (isDragging) {
      document.addEventListener('mousemove', handleMouseMove);
      document.addEventListener('mouseup', handleMouseUp);
    }

    return () => {
      document.removeEventListener('mousemove', handleMouseMove);
      document.removeEventListener('mouseup', handleMouseUp);
    };
  }, [isDragging, rotation, onRotationChange]);

  return (
    <div 
      ref={knobRef}
      className="size-[70px] cursor-grab active:cursor-grabbing select-none"
      onMouseDown={handleMouseDown}
      style={{ touchAction: 'none' }}
    >
      <svg className="block size-full" fill="none" preserveAspectRatio="none" viewBox="0 0 70 70">
        <g id="Knob">
          <g id="Ellipse 6">
            <mask fill="white" id={`path-1-inside-knob-${uniqueId}`}>
              <path d={svgPaths.p2b296300} />
            </mask>
            <path 
              d={svgPaths.p2b296300} 
              fill="var(--fill-0, #4D4D4D)" 
              mask={`url(#path-1-inside-knob-${uniqueId})`} 
              stroke="var(--stroke-0, #ABABAB)" 
            />
          </g>
          <g id="Group 1" style={{ transform: `rotate(${rotation}deg)`, transformOrigin: '35px 35px' }}>
            <circle 
              cx="35.25" 
              cy="35.25" 
              fill="var(--fill-0, #4D4D4D)" 
              id="Ellipse 5" 
              r="25.75" 
              stroke="var(--stroke-0, #C8C8C8)" 
            />
            <g filter={`url(#filter0_d_knob-${uniqueId})`} id="Line 1">
              <line 
                stroke="var(--stroke-0, #E2E2E2)" 
                strokeLinecap="round" 
                strokeWidth="2" 
                x1="35" 
                x2="35" 
                y1="20.7279" 
                y2="10" 
              />
            </g>
          </g>
        </g>
        <defs>
          <filter 
            colorInterpolationFilters="sRGB" 
            filterUnits="userSpaceOnUse" 
            height="20.7279" 
            id={`filter0_d_knob-${uniqueId}`}
            width="10" 
            x="30" 
            y="9"
          >
            <feFlood floodOpacity="0" result="BackgroundImageFix" />
            <feColorMatrix 
              in="SourceAlpha" 
              result="hardAlpha" 
              type="matrix" 
              values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0" 
            />
            <feOffset dy="4" />
            <feGaussianBlur stdDeviation="2" />
            <feComposite in2="hardAlpha" operator="out" />
            <feColorMatrix type="matrix" values="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.25 0" />
            <feBlend 
              in2="BackgroundImageFix" 
              mode="normal" 
              result="effect1_dropShadow_1_109" 
            />
            <feBlend 
              in="SourceGraphic" 
              in2="effect1_dropShadow_1_109" 
              mode="normal" 
              result="shape" 
            />
          </filter>
        </defs>
      </svg>
    </div>
  );
}